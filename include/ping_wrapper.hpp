#pragma once
#include <chrono>
#include <string>
#include <string_view>

#include "ping_stats.hpp"

class PingWrapper {
public:
  /**
   * Constructs a ping wrapper for the specified target
   * @param target_ip IPv4 address to ping
   * @throws std::invalid_argument if IP format is invalid
   */
  explicit PingWrapper(std::string_view target_ip);

  /**
   * Executes ping command and collects statistics
   * @param count Number of packets to send [2-50]
   * @param interval Time between packets [0.1-10ms]
   * @return Statistics for successful pings
   * @throws std::invalid_argument if parameters are invalid
   * @throws std::runtime_error if ping fails or too many packets lost
   */
  PingStats execute(int count,
                    std::chrono::duration<double, std::milli> interval);

private:
  std::string target_ip_;
};