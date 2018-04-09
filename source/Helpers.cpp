#include "Helpers.h"

Profiler::Profiler(std::string name)
    : _name(std::move(name))
    , _start{ Clock::now() }
{
}

Profiler::~Profiler()
{
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - _start );

    std::cout << "##teamcity[buildStatisticValue key='" << _name << "_logging_duration_ms' value='" << duration.count() << "']";
}
