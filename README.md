# Ping Advanced Minimal
A modern C++ wrapper around iputils ping providing statistical analysis.

## Requirements
- C++17 compiler
- Meson & Dependencies (for iputils)
- Make

## Project Structure
```
ping_adv_minimal/
├── external/
│   └── iputils/
│       └── build/
├── include/
│   ├── ping_stats.hpp
│   └── ping_wrapper.hpp
├── src/
│   ├── ping_stats.cpp
│   ├── ping_wrapper.cpp
│   └── main.cpp
├── README.md
└── CMakeLists.txt
```

## Setup (Ubuntu/Debian)
Install iputils dependencies:
```bash
# Relative location: ping_adv_minimal/
cd external/iputils/ci
chmod +x debian.sh
sudo ./debian.sh
cd ../../..
```

## Building
1. Build iputils (from project root):
```bash
# Relative location: ping_adv_minimal/
cd external/iputils
meson build
cd build && meson compile
cd ../../
```

2. Build our project (from project root):
```bash
# Relative location: ping_adv_minimal/
mkdir -p build && cd build
cmake ..
make
sudo cmake --install .
```

## Running
```bash
sudo ping_stats <target_ip> <count> <interval_ms>

Arguments:
  target_ip    - IP address to ping
  count        - Number of pings (2-50)
  interval_ms  - Time between pings (0.1-10ms)
```

## Example
```bash
sudo ping_stats 8.8.8.8 10 1.0
[8.8.8.8] Test Result: Average Latency 15.24 ms, Jitter 0.89 ms (10 results)
```

## Output Format
Success:
```
[IP] Test Result: Average Latency XXX.XX ms, Jitter YYY.YY ms (N results)
```
Failure:
```
[IP] Test Failed
```