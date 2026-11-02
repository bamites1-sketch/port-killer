# Port Killer

> A lightweight Windows CLI tool to identify processes listening on TCP ports.

**Current Status:** Milestone 1 - Inspection Only (v0.1.0)

Port Killer helps developers quickly identify which processes are using specific TCP ports on Windows systems. This is especially useful when encountering "port already in use" errors during development.

## Features

- ✅ Validate port arguments (1-65535)
- ✅ List TCP listeners by port
- ✅ Show process ID and name
- ✅ Clear error messages
- ❌ Process termination (planned for future milestone)

## Requirements

- Windows 10/11
- CMake 3.20+
- C++17 compatible compiler:
  - MSVC (Visual Studio 2017+) or
  - MinGW-w64 (via MSYS2 UCRT64) or
  - Clang

## Building

### Option 1: MSYS2 UCRT64 (MinGW-w64)

From MSYS2 UCRT64 terminal:

```bash
# Navigate to project directory
cd /c/Users/bamit/Development/port-killer

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .
```

The executable will be created at `build/port-killer.exe`.

### Option 2: Visual Studio / MSVC

From Developer Command Prompt or PowerShell:

```powershell
# Navigate to project directory
cd C:\Users\bamit\Development\port-killer

# Create and enter build directory
mkdir build
cd build

# Configure with MSVC
cmake ..

# Build the project
cmake --build . --config Release
```

The executable will be created at `build/Release/port-killer.exe`.

### Option 3: GitHub Actions CI

Every push to the repository automatically builds the project on Windows using MSVC in both Release and Debug configurations. Check the [Actions tab](../../actions) for build status and artifacts.

## Usage

```bash
# Inspect a specific port
./port-killer 8080

# Examples with different ports
./port-killer 80
./port-killer 3000
./port-killer 5432
```

### Example Output

```
====================================
      PORT INSPECTOR v0.1.0
====================================

Inspecting TCP port 8080...

Found 1 listener(s) on port 8080:

Local Address        Port    PID       Process Name
--------------------------------------------------------------------
0.0.0.0              8080    12345     node.exe

Note: This tool inspects only. No processes will be terminated.
```

## Error Handling

The tool handles several error cases gracefully:

- **Invalid port format**: Non-numeric or out-of-range values
- **No listeners found**: Port is not in use
- **Permission errors**: Some process details may require admin privileges
- **API failures**: Windows API errors are reported with error codes

## Limitations

1. **IPv4 only**: Currently inspects only IPv4 TCP listeners (IPv6 support planned)
2. **TCP only**: UDP inspection not yet implemented
3. **Process names**: Some system processes may show "[unable to retrieve]"
4. **Admin privileges**: Running as admin may provide more complete process information

## Security & Safety

⚠️ **Important Safety Notes:**

- This version **only inspects** ports - it does not terminate any processes
- Future versions with termination will require explicit confirmation
- Never terminate critical system processes (PID 0, 4, or other protected processes)
- Always verify the process before considering termination

### Windows Code Integrity

If Windows blocks execution with Smart App Control:
- The executable is unsigned (normal for local development)
- For testing, use an authorized environment like GitHub Actions Windows runner
- Do NOT disable Smart App Control or modify security policies
- Production releases should be properly code-signed

## Project Structure

```
port-killer/
├── CMakeLists.txt                    # Build configuration
├── README.md                         # This file
├── src/
│   ├── main.cpp                      # CLI entry point
│   ├── port_validator.h/.cpp         # Portable port validation
│   └── windows_port_inspector.h/.cpp # Windows API integration
└── build/                            # Build artifacts (gitignored)
```

## Development Philosophy

- **Beginner-friendly**: Clear code with educational comments
- **Safety-first**: No automatic termination, explicit confirmations
- **Portable where possible**: Platform-specific code isolated
- **Modern C++**: Using C++17 features and RAII
- **Minimal dependencies**: Uses only Windows API and C++ standard library

## Future Milestones

- [ ] Milestone 2: Interactive process termination with safety checks
- [ ] Milestone 3: IPv6 support
- [ ] Milestone 4: UDP port inspection
- [ ] Milestone 5: Batch processing and scripting support

## Contributing

This is a learning project. Feedback and suggestions are welcome! Feel free to:
- Report bugs or issues
- Suggest new features
- Submit pull requests
- Share feedback on code quality and design

## License

MIT License - See [LICENSE](LICENSE) file for details.

## Author

Built by [bamites1-sketch](https://github.com/bamites1-sketch) as a learning project for understanding Windows development and C++17.

## Acknowledgments

- Uses Windows IP Helper API for network inspection
- Built with CMake and modern C++17
- Tested with MSYS2 UCRT64 and MinGW-w64
