cmake_minimum_required(VERSION 3.10)
project(ping_stats VERSION 1.0)

# Compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Check for iputils dependency
set(IPUTILS_DIR "${PROJECT_SOURCE_DIR}/external/iputils")
if(NOT EXISTS "${IPUTILS_DIR}/build/ping/ping")
    message(FATAL_ERROR "iputils ping not found. Please build iputils first:\n"
            "cd external/iputils && meson build && cd build && meson compile")
endif()

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${IPUTILS_DIR}/include
)

# Add executable
add_executable(ping_stats
    src/main.cpp
    src/ping_stats.cpp
    src/ping_wrapper.cpp
)

# Install
install(TARGETS ping_stats
    RUNTIME DESTINATION bin
)

message(STATUS "Configuration completed for ping_stats")