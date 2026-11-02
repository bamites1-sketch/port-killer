# 🔍 Port Killer

> **Identify which process is using a port on Windows**

A lightweight CLI tool that helps developers quickly find processes listening on TCP ports.

[![Windows Build](https://github.com/bamites1-sketch/port-killer/actions/workflows/windows-build.yml/badge.svg)](https://github.com/bamites1-sketch/port-killer/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)

---

## ✨ Features

- 🎯 **Simple**: One command to find port conflicts
- ⚡ **Fast**: Native Windows API integration
- 🔒 **Safe**: Read-only inspection (no process termination)
- 📦 **Portable**: Single executable, no dependencies
- 🌐 **IPv6 Ready**: Supports both IPv4 and IPv6 TCP listeners

---

## 🚀 Quick Start

```bash
port-killer 8080
```

**Output:**
```
====================================
      PORT INSPECTOR v0.2.0
====================================

Inspecting TCP port 8080...

Found 2 listener(s) on port 8080:

Local Address                            Port    Type    PID       Process Name
------------------------------------------------------------------------------------------------
0.0.0.0                                  8080    IPv4    12345     node.exe
::                                       8080    IPv6    12345     node.exe

Note: This tool inspects only. No processes will be terminated.
```

---

## 📥 Installation

### Build from Source

**Requirements:** Windows 10/11, CMake 3.20+, MSVC or MinGW

```bash
git clone https://github.com/bamites1-sketch/port-killer.git
cd port-killer
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

---

## 💡 Use Cases

- 🐛 **Debug port conflicts** during development
- 🔍 **Identify mystery processes** using common ports
- 🛠️ **Quick troubleshooting** for "address already in use" errors

---

## 🗺️ Roadmap

- [x] **v0.1** - TCP port inspection (IPv4)
- [x] **v0.2** - IPv6 support ✨ NEW!
- [ ] **v0.3** - Interactive process termination
- [ ] **v0.4** - UDP inspection

---

## ⚠️ Current Limitations

- **TCP only** (no UDP yet)
- **Inspection only** (cannot terminate processes)
- Windows 10/11 only

---

## 🤝 Contributing

Found a bug? Have a feature request? [Open an issue](https://github.com/bamites1-sketch/port-killer/issues)!

---

## 📜 License

MIT © [bamites1-sketch](https://github.com/bamites1-sketch)

---

## 🛠️ Built With

- **C++17** - Modern C++ standards
- **Windows API** - Native IP Helper API (IPv4 & IPv6)
- **CMake** - Cross-compiler build system
- **GitHub Actions** - Automated CI/CD

---

<div align="center">

**Made with ❤️ for developers who hate port conflicts**

[Report Bug](https://github.com/bamites1-sketch/port-killer/issues) · [Request Feature](https://github.com/bamites1-sketch/port-killer/issues) · [View Actions](https://github.com/bamites1-sketch/port-killer/actions)

</div>
