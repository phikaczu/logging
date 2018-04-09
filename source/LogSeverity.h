#pragma once

#include <type_traits>
#include <ostream>

namespace logs2
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
}