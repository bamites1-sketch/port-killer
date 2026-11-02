#include "windows_port_inspector.h"

// Windows headers - order matters!
#include <winsock2.h>  // Must come before windows.h
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <tlhelp32.h>  // For process snapshot
#include <windows.h>

#include <memory>      // for std::unique_ptr
#include <sstream>     // for string formatting

// Link against Windows libraries
// Note: #pragma comment only works with MSVC, not MinGW
// For MinGW, libraries are linked via CMakeLists.txt
#ifdef _MSC_VER
    #pragma comment(lib, "iphlpapi.lib")
    #pragma comment(lib, "ws2_32.lib")
#endif

namespace portkiller {

std::string WindowsPortInspector::ip_to_string(uint32_t ip_address) {
    // IP addresses in the table are stored in network byte order (big-endian)
    // We need to extract each byte
    unsigned char byte1 = (ip_address) & 0xFF;
    unsigned char byte2 = (ip_address >> 8) & 0xFF;
    unsigned char byte3 = (ip_address >> 16) & 0xFF;
    unsigned char byte4 = (ip_address >> 24) & 0xFF;

    std::ostringstream oss;
    oss << static_cast<int>(byte1) << "."
        << static_cast<int>(byte2) << "."
        << static_cast<int>(byte3) << "."
        << static_cast<int>(byte4);
    
    return oss.str();
}

std::string WindowsPortInspector::get_process_name(uint32_t pid) {
    // Special case: PID 0 and 4 are system processes
    if (pid == 0) {
        return "[System Idle Process]";
    }
    if (pid == 4) {
        return "[System]";
    }

    // Take a snapshot of all processes
    // This is like a "photograph" of all running processes at this moment
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return "";  // Can't get process info
    }

    // RAII wrapper - ensures we close the handle even if we return early
    std::unique_ptr<void, decltype(&CloseHandle)> snapshot_guard(snapshot, CloseHandle);

    PROCESSENTRY32W process_entry;
    process_entry.dwSize = sizeof(PROCESSENTRY32W);

    // Get the first process
    if (!Process32FirstW(snapshot, &process_entry)) {
        return "";
    }

    // Walk through all processes looking for matching PID
    do {
        if (process_entry.th32ProcessID == pid) {
            // Found it! Convert wide string to regular string
            // Windows uses wide characters (wchar_t) for Unicode
            int size_needed = WideCharToMultiByte(CP_UTF8, 0, 
                process_entry.szExeFile, -1, nullptr, 0, nullptr, nullptr);
            
            if (size_needed <= 0) {
                return "";
            }

            std::string result(size_needed - 1, '\0');  // -1 to exclude null terminator
            WideCharToMultiByte(CP_UTF8, 0, process_entry.szExeFile, -1,
                &result[0], size_needed, nullptr, nullptr);
            
            return result;
        }
    } while (Process32NextW(snapshot, &process_entry));

    return "";  // PID not found
}

InspectionResult WindowsPortInspector::find_listeners(int port) {
    // Convert port to network byte order (big-endian) for comparison
    // Windows stores ports in network byte order
    uint16_t target_port = htons(static_cast<uint16_t>(port));

    // First call: find out how much memory we need
    DWORD table_size = 0;
    DWORD result = GetExtendedTcpTable(
        nullptr,           // null buffer to query size
        &table_size,       // receives required size
        FALSE,             // no sorting needed
        AF_INET,           // IPv4 (we're keeping it simple for now)
        TCP_TABLE_OWNER_PID_LISTENER,  // Only listening ports
        0                  // reserved
    );

    if (result != ERROR_INSUFFICIENT_BUFFER) {
        return {false, {}, "Failed to query TCP table size (error code: " + 
                std::to_string(result) + ")"};
    }

    // Allocate memory for the table
    std::vector<char> buffer(table_size);
    auto* tcp_table = reinterpret_cast<MIB_TCPTABLE_OWNER_PID*>(buffer.data());

    // Second call: actually get the data
    result = GetExtendedTcpTable(
        tcp_table,
        &table_size,
        FALSE,
        AF_INET,
        TCP_TABLE_OWNER_PID_LISTENER,
        0
    );

    if (result != NO_ERROR) {
        return {false, {}, "Failed to retrieve TCP table (error code: " + 
                std::to_string(result) + ")"};
    }

    // Now search through all TCP listeners for matching port
    std::vector<TcpListener> found_listeners;

    for (DWORD i = 0; i < tcp_table->dwNumEntries; ++i) {
        const auto& row = tcp_table->table[i];

        // Check if this listener matches our target port
        if (row.dwLocalPort == target_port) {
            TcpListener listener;
            listener.local_address = ip_to_string(row.dwLocalAddr);
            listener.local_port = ntohs(static_cast<uint16_t>(row.dwLocalPort));
            listener.process_id = row.dwOwningPid;
            listener.process_name = get_process_name(row.dwOwningPid);

            found_listeners.push_back(listener);
        }
    }

    return {true, found_listeners, ""};
}

} // namespace portkiller
