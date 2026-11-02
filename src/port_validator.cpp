#include "port_validator.h"
#include <cctype>     // for std::isdigit
#include <stdexcept>  // for std::out_of_range

namespace portkiller {

PortValidationResult PortValidator::validate(const std::string& port_arg) {
    // Check for empty input
    if (port_arg.empty()) {
        return {false, 0, "Port argument cannot be empty"};
    }

    // Check that all characters are digits
    // We don't use std::stoi directly because it can throw exceptions
    // and accepts things like "123abc" (stops at first non-digit)
    for (char c : port_arg) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return {false, 0, "Port must be a valid integer (found non-digit character)"};
        }
    }

    // Now we know it's all digits, safe to convert
    // Still need to check for overflow (e.g., "999999999999")
    try {
        // std::stoi throws std::out_of_range if number is too large
        int port = std::stoi(port_arg);

        // Validate port range
        if (port < MIN_PORT || port > MAX_PORT) {
            return {false, 0, "Port must be between 1 and 65535"};
        }

        return {true, port, ""};
    }
    catch (const std::out_of_range&) {
        return {false, 0, "Port number is too large (must be between 1 and 65535)"};
    }
    catch (...) {
        // Shouldn't happen since we validated digits, but be defensive
        return {false, 0, "Failed to parse port number"};
    }
}

} // namespace portkiller
