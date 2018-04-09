#pragma once

#include <string>
#include "LogSettings.h"

namespace logs2
{
    class Sink final
    {
        // Pure static
        Sink() = delete;
        ~Sink() = delete;

    public:
        static void initSink(Appender& appender);
        static void log(const std::string& message);
        static void log(std::string&& message);
    };
}