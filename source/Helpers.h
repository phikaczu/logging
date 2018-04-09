#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>

inline std::ostream& niceHexa(std::ostream& os)
{
    os << "0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0');
    return os;
}

class Profiler final
{
public:
    using Clock = std::chrono::high_resolution_clock;

    Profiler(std::string name);
    ~Profiler();

private:
    const std::string _name;
    const Clock::time_point _start;
};
