#pragma once

#include <memory>
#include "logger.h"
#include "LogSettings.h"

namespace logs2
{
    class Log final
    {
    public:
        explicit Log(std::shared_ptr<LogSettings> settings);

        Logger operator()(Severity severity) const;

    private:
        std::shared_ptr<LogSettings> m_settings;
    };
}
