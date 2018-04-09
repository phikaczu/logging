#pragma once

#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include "LogSeverity.h"
#include "appender.h"
#include "LogSink.h"

namespace logs2
{
    class Log;

    /// Provides logging of messages using a stream operator.
    template <class Check>
    class LoggerTemplate final : private Check
    {
    private:
        friend Log;

        /// Type used for the internal message buffer
        using Buffer = std::stringstream;
        // Some type aliases to simplify method signatures
        using OStreamType = std::basic_ostream<Buffer::char_type, Buffer::traits_type>;
        using IOStreamType = std::basic_ios<Buffer::char_type, Buffer::traits_type>;
        using OManip = OStreamType & (*)(OStreamType &);
        using IOManip = IOStreamType & (*)(IOStreamType &);

    public:
        LoggerTemplate(LoggerTemplate &&) = default;
        LoggerTemplate &operator=(LoggerTemplate &&) = default;

        LoggerTemplate(const LoggerTemplate &) = delete;
        LoggerTemplate &operator=(const LoggerTemplate &) = delete;

        /// Destructs the instance
        /// @note Synchronizes with the message logger, so all messages will be logged before destruction.
        ~LoggerTemplate();

    private:
        /// Constructs a new instance.
        /// @param severity the message severity
        /// @param isFake if \c true, then the logger will act as a null sink, else it will log
        LoggerTemplate(Severity severity, Appender& appender, bool isFake);

    public:
        /// Writes a string representation of @p value to the message.
        /// @tparam T type of the @p value; it is always deduced
        /// @param value instance to be written to the message
        /// @note The @p value is perfect forwarded to the LogHelper::Buffer, i.e. all rules for the @c
        /// LogHelper::Buffer::operator<< do apply.
        template <typename T>
        inline LoggerTemplate &operator<<(T &&value);

        /// Calls <tt>func(*this)</tt>;. This overloads is used to implement output I/O manipulators such as std::endl.
        /// @param func function to call
        LoggerTemplate &operator<<(OManip func);
        /// @copydoc LoggerTemplate::operator<<(OManip)
        LoggerTemplate &operator<<(IOManip func);

        /// Synchronizes with the underlying message buffer.
        void Flush();

    private:
        /// Writes a message to the underlying message buffer.
        /// @param message string to be written
        void Log(std::string &&message) const;

        const Severity m_severity;                  ///< severity to use for the messages
        std::unique_ptr<Buffer> m_messageBuffer;    ///< buffer to use for the messages
    };

    template <class Check>
    LoggerTemplate<Check>::LoggerTemplate(Severity severity, Appender& appender, bool isFake)
        : m_severity{ severity }
        , m_messageBuffer(isFake ? nullptr : std::make_unique<Buffer>())
    {
        Sink::initSink(appender);
    }

    template <class Check>
    LoggerTemplate<Check>::~LoggerTemplate()
    {
        Flush();
    }

    template <class Check>
    template <typename T>
    LoggerTemplate<Check> &LoggerTemplate<Check>::operator<<(T &&value)
    {
        if (m_messageBuffer)
        {
            *m_messageBuffer << std::forward<T>(value);
        }

        return *this;
    }

    template <class Check>
    LoggerTemplate<Check> &LoggerTemplate<Check>::operator<<(OManip func)
    {
        if (!m_messageBuffer) return *this;

        func(*m_messageBuffer);

        if (func == &std::endl<Buffer::char_type, Buffer::traits_type> ||
            func == &std::flush<Buffer::char_type, Buffer::traits_type>)
        {
            Flush();
        }

        return *this;
    }

    template <class Check>
    LoggerTemplate<Check> &LoggerTemplate<Check>::operator<<(IOManip func)
    {
        if (m_messageBuffer)
        {
            func(*m_messageBuffer);
        }

        return *this;
    }

    template <class Check>
    void LoggerTemplate<Check>::Flush()
    {
        if (!m_messageBuffer) return;

        // OPEN : A string is created to check if the stream is not empty.
        // According to this answer https://stackoverflow.com/a/34651454 the following expression
        // can do for not yet extracted characters
        //  m_messageBuffer->peek() == Buffer::traits_type::eof()
        // But because the temporary string is further used for logging, the existing solution
        // is probably not sub-optimal, provided that for an empty stream the string construction
        // is cheap.
        // There can be also other possibilities, like give the whole Buffer to the Log ...
        auto msg = m_messageBuffer->str();
        if (!msg.empty()) {
            Log(std::move(msg));
            // sets an empty string as the content of the stream, i.e. effectively erases the content
            m_messageBuffer->str(std::string());
            // clears all error flags, so the stream is as good as new
            m_messageBuffer->clear();
        }
    }

    template <class Check>
    void LoggerTemplate<Check>::Log(std::string &&message) const
    {
        Check::isSameThread();
        Sink::log(std::move(message));
    }
}