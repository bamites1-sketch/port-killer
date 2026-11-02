
cmake_minimum_required(VERSION 3.20)

project(PortKiller
    VERSION 0.1.0
    DESCRIPTION "A safe Windows port inspection utility"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(port-killer
    src/main.cpp
    src/port_validator.cpp
    src/windows_port_inspector.cpp
)

# Link Windows system libraries for networking and process inspection
if(WIN32)
    target_link_libraries(port-killer PRIVATE iphlpapi ws2_32)
endif()

if(MSVC)
    target_compile_options(port-killer PRIVATE /W4)
else()
    target_compile_options(port-killer PRIVATE -Wall -Wextra -Wpedantic)
endif()
