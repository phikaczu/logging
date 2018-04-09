#include <gtest\gtest.h>

#include "appender.h"
#include "LogSettings.h"
#include "log.h"

namespace
{
    struct ThreadArray
    {
        template<typename Func>
        ThreadArray(std::size_t threadsCount, Func&& fun);
        ~ThreadArray();

    private:
        const std::size_t _threadsCount;
        std::vector<std::unique_ptr<std::thread>> _ths;
    };

    template<typename Func>
    ThreadArray::ThreadArray(std::size_t threadsCount, Func && fun)
        : _threadsCount{ threadsCount }
    {
        _ths.reserve(_threadsCount);
        for (std::size_t idx = 0; idx < _threadsCount; ++idx)
        {
            _ths.push_back(std::make_unique<std::thread>(fun));
        }
    }

    ThreadArray::~ThreadArray()
    {
        for (auto& th : _ths)
        {
            th->join();
        }
    }

    template <typename F>
    void runParallel(F&& func, std::size_t threadsCount, std::size_t repetitionPerThread)
    {
        auto repeater = [&]()
        {
            for (std::size_t rep = 0; rep < repetitionPerThread; ++rep)
            {
                func(rep);
            }
        };

        ThreadArray threads(threadsCount, repeater);
    }
}

TEST(appenderTest, basic)
{
    auto settings = std::make_shared<logs2::LogSettings>();
    settings->severity = logs2::Severity::Error | logs2::Severity::Warning;
    // Default appender is automatically created

    auto log = logs2::Log(settings);
    auto testLog = [&](std::size_t rep)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        log(logs2::Severity::Error) << "Log message from thread " << std::this_thread::get_id() << "; idx = " << rep << '\n';
    };

    runParallel(testLog, 4, 500);
}