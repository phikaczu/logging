#include <gtest\gtest.h>

#include "log.h"
#include "Helpers.h"

TEST(logging2test, basic)
{
    auto settings = std::make_shared<logs2::LogSettings>();
    settings->severity = logs2::Severity::Error | logs2::Severity::Warning;
    auto log = logs2::Log(settings);

    {
        Profiler profile("log2");
        for (int i = 0; i < 100000; ++i)
        {
            log(logs2::Severity::Info) << "Hello world" << std::endl;
            log(logs2::Severity::Error) << "code = " << niceHexa << 15 << std::endl;
        }
    }
}
