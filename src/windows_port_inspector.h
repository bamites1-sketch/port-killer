#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace portkiller {

// Represents a TCP listener found on the system
struct TcpListener {
    std::string local_address;  // e.g., "0.0.0.0" or "127.0.0.1"
    int local_port;
    uint32_t process_id;
    std::string process_name;   // May be empty if we can't retrieve it
};

// Result of a port inspection operation
struct InspectionResult {
    bool success;
    std::vector<TcpListener> listeners;
    std::string error_message;  // Only set if success == false
};

// Windows-specific port inspection using Windows API
// Isolates all Windows dependencies to this class
class WindowsPortInspector {
public:
    // Find all TCP listeners on the specified port
    // Returns structured result with listeners or error message
    static InspectionResult find_listeners(int port);

private:
    // Helper: Convert numeric IP address to string (e.g., 0x0100007f -> "127.0.0.1")
    static std::string ip_to_string(uint32_t ip_address);
    
    // Helper: Get process name from PID, returns empty string on failure
    static std::string get_process_name(uint32_t pid);
};

} // namespace portkiller
