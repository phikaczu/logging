#include <gtest\gtest.h>

#include "Logging.h"
#include "Helpers.h"

TEST(loggingtest, basic)
{
    auto settings = std::make_shared<logs::LogSettings>();
    settings->severity = logs::Severity::Error | logs::Severity::Warning;
    auto log = logs::Log(settings);

    {
        Profiler profile("log");
        for (int i = 0; i < 100000; ++i)
        {
            log(logs::Severity::Info) << "Hello world" << std::endl;
            log(logs::Severity::Error) << "code = " << niceHexa << 15 << std::endl;
        }
    }
}
