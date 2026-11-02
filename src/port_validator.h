#pragma once

#include <string>
#include <optional>

namespace portkiller {

// Represents the result of validating a port argument
struct PortValidationResult {
    bool is_valid;
    int port_number;      // Only meaningful if is_valid == true
    std::string error_message;  // Only meaningful if is_valid == false
};

// Pure portable C++ class - no Windows dependencies
// This makes it easy to unit test and understand
class PortValidator {
public:
    // Validates a port string argument
    // Returns structured result indicating success/failure
    static PortValidationResult validate(const std::string& port_arg);

private:
    static constexpr int MIN_PORT = 1;
    static constexpr int MAX_PORT = 65535;
};

} // namespace portkiller
