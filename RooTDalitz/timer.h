#pragma once

#include <chrono>
#include <string>

namespace Time{
    struct Timer{
        std::chrono::time_point<std::chrono::steady_clock> start_time, finish_time;

        Timer() = default;
        ~Timer() = default;

        void print_current_date_time(std::string message);
        void start();
        void finish();
        void duration();
    };
} // namespace Timer
