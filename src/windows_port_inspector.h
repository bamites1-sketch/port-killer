#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace portkiller {

// Represents a TCP listener found on the system
struct TcpListener {
    std::string local_address;  // e.g., "0.0.0.0", "127.0.0.1", "::", "::1"
    int local_port;
    uint32_t process_id;
    std::string process_name;   // May be empty if we can't retrieve it
    bool is_ipv6;               // true for IPv6, false for IPv4
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
    // Find all TCP listeners on the specified port (both IPv4 and IPv6)
    // Returns structured result with listeners or error message
    static InspectionResult find_listeners(int port);

private:
    // Helper: Find IPv4 TCP listeners on specified port
    static InspectionResult find_ipv4_listeners(int port);
    
    // Helper: Find IPv6 TCP listeners on specified port
    static InspectionResult find_ipv6_listeners(int port);
    
    // Helper: Convert numeric IPv4 address to string (e.g., 0x0100007f -> "127.0.0.1")
    static std::string ipv4_to_string(uint32_t ip_address);
    
    // Helper: Convert numeric IPv6 address to string (e.g., "::1", "fe80::1")
    static std::string ipv6_to_string(const unsigned char* ipv6_address);
    
    // Helper: Get process name from PID, returns empty string on failure
    static std::string get_process_name(uint32_t pid);
};

} // namespace portkiller
