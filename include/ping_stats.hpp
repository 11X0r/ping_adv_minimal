#pragma once
#include <chrono>
#include <cstddef>
#include <vector>

class PingStats {
public:
  void add_result(std::chrono::nanoseconds latency);
  bool is_valid() const noexcept { return latencies_.size() >= 2; }
  std::size_t count() const noexcept { return latencies_.size(); }
  double average_latency_ms() const noexcept;
  double jitter_ms() const noexcept;

private:
  std::vector<std::chrono::nanoseconds> latencies_;
};
