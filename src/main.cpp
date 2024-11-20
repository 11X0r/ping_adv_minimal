#include <iomanip>
#include <iostream>

#include "ping_wrapper.hpp"

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <target_ip> <count> <interval_ms>\n";
    return 1;
  }

  try {
    const std::string_view target_ip = argv[1];
    const int count = std::stoi(argv[2]);
    const double interval = std::stod(argv[3]);

    PingWrapper ping(target_ip);
    const auto stats = ping.execute(
        count, std::chrono::duration<double, std::milli>(interval));

    std::cout << std::fixed << std::setprecision(2);

    if (stats.is_valid()) {
      std::cout << "[" << target_ip << "] Test Result: " << "Average Latency "
                << stats.average_latency_ms() << "ms, " << "Jitter "
                << stats.jitter_ms() << "ms " << "(" << stats.count()
                << " results)\n";
    } else {
      std::cout << "[" << target_ip << "] Test Failed\n";
    }
    return 0;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
