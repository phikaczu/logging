#include "appender.h"

#include <iostream>

using namespace logs2;

const std::chrono::milliseconds Appender::_checkPeriod(250);

logs2::Appender::Appender()
    : _run{ true }
    , _sinkTh(&Appender::sinkThreadMethod, this)
{
}

logs2::Appender::~Appender()
{
    {
        std::unique_lock<std::mutex> lk(_runCvMtx);
        _run = false;
        _runCv.notify_one();
    }

    _sinkTh.join();
}

void Appender::registerSinkManager(SinkManager& manager)
{
    {
        std::lock_guard<std::mutex> lk(_sinksMtx);
        _sinks.push_back(manager.getSink());
    }

    // OPEN does this class need a notification?
    //manager.setFinalizer([&](std::unique_ptr<SinkQueue>&& queue)
    //{
    //    std::lock_guard<std::mutex> lk(_finishedSinksMtx);
    //    _finishedSinks.push_back(std::move(queue));
    //});
}

void logs2::Appender::sinkThreadMethod()
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lk(_runCvMtx);
            if (!_run) break;
            (void)_runCv.wait_for(lk, _checkPeriod);
        }

        cleanSinks();
    }
}

void Appender::cleanSinks()
{
    SinkContainer sinks;

    {
        std::lock_guard<std::mutex> lk(_sinksMtx);
        sinks.insert(sinks.end(), _sinks.cbegin(), _sinks.cend());

        // Remove abandoned sinks (sinks for which the source thread is no longer alive)
        _sinks.erase(
            std::remove_if(
                _sinks.begin(),
                _sinks.end(), 
                [](const SinkContainer::value_type& item)
                {
                    return item.use_count() == 2;
                }),
            _sinks.cend());
    }

    for (auto& sink : sinks)
    {
        std::string message;
        while (sink->pop(message))
        {
            std::cout << message;
        }
    }

}