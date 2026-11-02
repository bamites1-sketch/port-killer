
#include <iostream>
#include <iomanip>  // for std::setw formatting
#include "port_validator.h"
#include "windows_port_inspector.h"

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <port>\n";
    std::cout << "  port: TCP port number to inspect (1-65535)\n\n";
    std::cout << "Example: " << program_name << " 8080\n";
}

int main(int argc, char* argv[]) {
    std::cout << "====================================\n";
    std::cout << "      PORT INSPECTOR v0.2.0\n";
    std::cout << "====================================\n\n";

    // Check command-line arguments
    if (argc != 2) {
        std::cerr << "Error: Expected exactly one argument\n\n";
        print_usage(argv[0]);
        return 1;
    }

    // Step 1: Validate the port argument
    std::string port_arg = argv[1];
    auto validation = portkiller::PortValidator::validate(port_arg);

    if (!validation.is_valid) {
        std::cerr << "Error: " << validation.error_message << "\n\n";
        print_usage(argv[0]);
        return 1;
    }

    int port = validation.port_number;
    std::cout << "Inspecting TCP port " << port << "...\n\n";

    // Step 2: Find listeners on that port
    auto inspection = portkiller::WindowsPortInspector::find_listeners(port);

    if (!inspection.success) {
        std::cerr << "Error: " << inspection.error_message << "\n";
        return 1;
    }

    // Step 3: Display results
    if (inspection.listeners.empty()) {
        std::cout << "No TCP listeners found on port " << port << "\n";
        return 0;
    }

    std::cout << "Found " << inspection.listeners.size() 
              << " listener(s) on port " << port << ":\n\n";

    // Print table header
    std::cout << std::left 
              << std::setw(40) << "Local Address"
              << std::setw(8) << "Port"
              << std::setw(8) << "Type"
              << std::setw(10) << "PID"
              << std::setw(30) << "Process Name"
              << "\n";
    std::cout << std::string(96, '-') << "\n";

    // Print each listener
    for (const auto& listener : inspection.listeners) {
        std::cout << std::left
                  << std::setw(40) << listener.local_address
                  << std::setw(8) << listener.local_port
                  << std::setw(8) << (listener.is_ipv6 ? "IPv6" : "IPv4")
                  << std::setw(10) << listener.process_id
                  << std::setw(30) << (listener.process_name.empty() 
                      ? "[unable to retrieve]" 
                      : listener.process_name)
                  << "\n";
    }

    std::cout << "\nNote: This tool inspects only. No processes will be terminated.\n";

    return 0;
}
