#include "ping_stats.hpp"

#include <cstdlib>
#include <numeric>

void PingStats::add_result(std::chrono::nanoseconds latency) {
  latencies_.push_back(latency);
}

double PingStats::average_latency_ms() const noexcept {
  if (latencies_.empty()) {
    return 0.0;
  }

  auto sum = std::accumulate(latencies_.begin(), latencies_.end(),
                             std::chrono::nanoseconds::zero());
  return std::chrono::duration<double, std::milli>(sum / latencies_.size())
      .count();
}

double PingStats::jitter_ms() const noexcept {
  if (latencies_.size() < 2) {
    return 0.0;
  }

  std::chrono::nanoseconds total_diff(0);
  for (std::size_t i = 1; i < latencies_.size(); ++i) {
    total_diff += std::chrono::nanoseconds(
        std::abs((latencies_[i] - latencies_[i - 1]).count()));
  }

  return std::chrono::duration<double, std::milli>(total_diff /
                                                   (latencies_.size() - 1))
      .count();
}
