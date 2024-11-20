#include "ping_wrapper.hpp"
#include <stdexcept>
#include <array>
#include <memory>
#include <sstream>
#include <regex>
#include <system_error>

namespace {
    struct PipeDeleter {
        void operator()(FILE* pipe) const noexcept {
            if (pipe) pclose(pipe);
        }
    };

    constexpr int MIN_COUNT = 2;
    constexpr int MAX_COUNT = 50;
    constexpr double MIN_INTERVAL_MS = 0.1;
    constexpr double MAX_INTERVAL_MS = 10.0;
    constexpr int TIMEOUT_SECONDS = 1;
    constexpr size_t BUFFER_SIZE = 128;

    bool validate_ip(std::string_view ip) noexcept {
        static const std::regex ip_regex(
            R"(^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3})"
            R"((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)"
        );
        return std::regex_match(ip.begin(), ip.end(), ip_regex);
    }

    std::string build_ping_command(std::string_view ip, int count, double interval_ms) {
        std::stringstream cmd;
        cmd << "ping"
            << " -c " << count
            << " -i " << interval_ms/1000.0
            << " -W " << TIMEOUT_SECONDS
            << " -n"
            << " " << ip
            << " 2>&1";
        return cmd.str();
    }
}

PingWrapper::PingWrapper(std::string_view target_ip) {
    if (!validate_ip(target_ip)) {
        throw std::invalid_argument("Invalid IP address format");
    }
    target_ip_ = std::string(target_ip);
}

PingStats PingWrapper::execute(int count, std::chrono::duration<double, std::milli> interval) {
    // Validate parameters
    if (count < MIN_COUNT || count > MAX_COUNT) {
        throw std::invalid_argument(
            "Count must be between " + std::to_string(MIN_COUNT) + 
            " and " + std::to_string(MAX_COUNT)
        );
    }

    const auto interval_ms = interval.count();
    if (interval_ms < MIN_INTERVAL_MS || interval_ms > MAX_INTERVAL_MS) {
        throw std::invalid_argument(
            "Interval must be between " + std::to_string(MIN_INTERVAL_MS) + 
            "ms and " + std::to_string(MAX_INTERVAL_MS) + "ms"
        );
    }

    // Execute ping command
    std::unique_ptr<FILE, PipeDeleter> pipe(
        popen(build_ping_command(target_ip_, count, interval_ms).c_str(), "r")
    );
    
    if (!pipe) {
        throw std::system_error(
            errno, std::system_category(), 
            "Failed to execute ping command"
        );
    }

    // Read command output
    std::array<char, BUFFER_SIZE> buffer;
    std::string output;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        output += buffer.data();
    }

    // Parse results
    static const std::regex time_regex(R"(time=([0-9.]+) ms)");
    std::string::const_iterator search_start = output.cbegin();
    const int max_failures = std::min(5, count / 2);
    int failures = 0;
    PingStats stats;
    std::smatch match;

    while (std::regex_search(search_start, output.cend(), match, time_regex)) {
        try {
            double ms = std::stod(match[1].str());
            auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::duration<double, std::milli>(ms)
            );
            stats.add_result(ns);
        } catch (const std::exception&) {
            if (++failures >= max_failures) break;
        }
        search_start = match.suffix().first;
    }

    if (!stats.is_valid() || failures >= max_failures) {
        throw std::runtime_error("Excessive ping failures");
    }

    return stats;
}