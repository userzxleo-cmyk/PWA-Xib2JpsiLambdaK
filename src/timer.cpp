#include <iostream>
#include <iomanip>

#include "RooTDalitz/timer.h"

namespace Time{
    void Timer::print_current_date_time(std::string message) {
        const auto now = std::chrono::system_clock::now();
        const auto time = std::chrono::system_clock::to_time_t(now);
        std::cout << message << ": " << std::put_time(std::localtime(&time), "Date: %Y.%m.%d Time: %H:%M:%S") << std::endl;
    }

    void Timer::start(){
        start_time = std::chrono::steady_clock::now();
        print_current_date_time("Start Time");
    }

    void Timer::finish(){
        finish_time = std::chrono::steady_clock::now();
        print_current_date_time("Finish Time");
    }

    void Timer::duration(){
        const auto difference = finish_time - start_time;
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(difference).count();
        std::cout << "Took " << seconds << " seconds!" << std::endl;
    }
} // namespace Timer
