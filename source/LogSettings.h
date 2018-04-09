#pragma once

#include "LogSeverity.h"
#include "appender.h"

namespace logs2
{
    struct LogSettings final
    {
        Severity severity;
        Appender appender;
    };
}