#pragma once

#include <memory>
#include <ostream>
#include <sstream>
#include <string>

namespace logs
{
    /// Represents possible log severities.
    enum class Severity
    {
        // 0 Reserved
        Error = 1 << 0,
        Warning = 1 << 1,
        Debug = 1 << 2,
        Info = 1 << 3
    };

    inline constexpr Severity operator | (Severity lhs, Severity rhs)
    {
        using T = std::underlying_type_t <Severity>;
        return static_cast<Severity>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    inline Severity& operator |= (Severity& lhs, Severity rhs)
    {
        using T = std::underlying_type_t <Severity>;
        lhs = static_cast<Severity>(static_cast<T>(lhs) | static_cast<T>(rhs));
        return lhs;
    }

    inline constexpr Severity operator & (Severity lhs, Severity rhs)
    {
        using T = std::underlying_type_t <Severity>;
        return static_cast<Severity>(static_cast<T>(lhs) & static_cast<T>(rhs));
    }

    inline std::ostream& operator<< (std::ostream& os, Severity sev)
    {
        switch (sev)
        {
        case Severity::Error:   os << "Error";   break;
        case Severity::Warning: os << "Warning"; break;
        case Severity::Debug:   os << "Debug";   break;
        case Severity::Info:    os << "Info";    break;
        default:
            os.setstate(std::ios_base::failbit);
        }

        return os;
    }

    struct LogSettings final
    {
        Severity severity;
    };

    class Log;

    /// Provides logging of messages using a stream operator.
    class Logger final
    {
    private:
        friend Log;

        /// Type used for the internal message buffer
        using Buffer = std::ostream;
        using LoggBuffer = std::stringstream;
        // Some type aliases to simplify method signatures
        using OStreamType = std::basic_ostream<Buffer::char_type, Buffer::traits_type>;
        using IOStreamType = std::basic_ios<Buffer::char_type, Buffer::traits_type>;
        using OManip = OStreamType &(*)(OStreamType &);
        using IOManip = IOStreamType &(*)(IOStreamType &);

    public:
        Logger(Logger &&) = default;
        Logger &operator=(Logger &&) = default;

        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete;

        /// Destructs the instance
        /// @note Synchronizes with the message logger, so all messages will be logged before destruction.
        ~Logger();

    private:
        /// Constructs a new instance.
        /// @param severity the message severity
        /// @param isFake if \c true, then the logger will act as a null sink, else it will log
        Logger(Severity severity, bool isFake);

    public:
        /// Writes a string representation of @p value to the message.
        /// @tparam T type of the @p value; it is always deduced
        /// @param value instance to be written to the message
        /// @note The @p value is perfect forwarded to the LogHelper::Buffer, i.e. all rules for the @c
        /// LogHelper::Buffer::operator<< do apply.
        template <typename T>
        inline Logger &operator<<(T &&value);

        /// Calls <tt>func(*this)</tt>;. This overloads is used to implement output I/O manipulators such as std::endl.
        /// @param func function to call
        Logger &operator<<(OManip func);
        /// @copydoc Logger::operator<<(OManip)
        Logger &operator<<(IOManip func);

        /// Synchronizes with the underlying message buffer.
        void Flush();

    private:
        /// Writes a message to the underlying message buffer.
        /// @param message string to be written
        void Log(const std::string &message) const;

        const Severity m_severity;                  ///< severity to use for the messages
        const bool m_isFake;
        std::unique_ptr<Buffer> m_messageBuffer;    ///< buffer to use for the messages
    };

    class Log final
    {
    public:
        explicit Log(std::shared_ptr<LogSettings> settings);

        Logger operator()(Severity severity) const;

    private:
        std::shared_ptr<LogSettings> m_settings;
    };


    template <typename T>
    Logger &Logger::operator<<(T &&value)
    {
        // it is actually faster to test the m_isFake than to call into the null sink std::ostream(nullptr)
        // see the Logging2 class
        *m_messageBuffer << std::forward<T>(value);
        return *this;
    }
}
