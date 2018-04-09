#include "Logging.h"

#include <iostream>

using namespace logs;

Logger::Logger(Severity severity, bool isFake)
    : m_severity{ severity }
    , m_isFake{ isFake }
    , m_messageBuffer( isFake ? std::make_unique<Buffer>(nullptr) : std::make_unique<LoggBuffer>() )
{
}


Logger::~Logger()
{
    Flush();
}

Logger &Logger::operator<<(Logger::OManip func)
{
    func(*m_messageBuffer);

    if (func == &std::endl<Buffer::char_type, Buffer::traits_type> ||
        func == &std::flush<Buffer::char_type, Buffer::traits_type>)
    {
        Flush();
    }

    return *this;
}

Logger &Logger::operator<<(Logger::IOManip func)
{
    func(*m_messageBuffer);

    return *this;
}

void Logger::Flush()
{
    if (m_isFake) return;

    auto& buffer = dynamic_cast<LoggBuffer&>(*m_messageBuffer.get());

    // OPEN : there exists no way how to find out, if there are any data in the stream buffer,
    // so a string is created, this is suboptimal
    const auto msg = buffer.str();
    if (!msg.empty()) {
        Log(msg);
        // sets an empty string as the content of the stream, i.e. effectively erases the content
        buffer.str(std::string());
        // clears all error flags, so the stream is as good as new
        buffer.clear();
    }
}

void Logger::Log(const std::string &str) const
{
    std::cout << '[' << m_severity << "] " << str;
}

Log::Log(std::shared_ptr<LogSettings> settings)
    : m_settings(std::move(settings))
{
}

Logger Log::operator()(Severity severity) const
{
    const auto logSeverity = (m_settings->severity & severity) == severity;
    return Logger(severity, !logSeverity);
}
