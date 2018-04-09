#include "log.h"

#include <iostream>
#include "LogSink.h"

using namespace logs2;

Log::Log(std::shared_ptr<LogSettings> settings)
    : m_settings(std::move(settings))
{
}

Logger Log::operator()(Severity severity) const
{
    const auto logSeverity = (m_settings->severity & severity) == severity;
    return Logger(severity, m_settings->appender, !logSeverity);
}
