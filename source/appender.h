#pragma once

#include <functional>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include "SinkManager.h"

namespace logs2
{
    class Appender final
    {
    public:
        Appender();
        ~Appender();
        void registerSinkManager(SinkManager& sinkMgr);

        // TODO add a start method

    private:
        using SinkContainer = std::vector<std::shared_ptr<SinkQueue>>;

        void sinkThreadMethod();
        void cleanSinks();

        static const std::chrono::milliseconds _checkPeriod;
        bool _run;
        std::condition_variable _runCv;
        std::mutex _runCvMtx;
        mutable std::mutex _sinksMtx;
        SinkContainer _sinks;
        std::thread _sinkTh;    // must be the last

        // TODO
        // Bylo by super udelat nasledujici typy vycitani sinku
        // * Periodicke - vycitaci vlakno se periodicky budi a provadi praci ... perioda se muze prizpusobovat
        // * Check and Signal - existuje atomic_bool isRunning, pri pridani do sinkMgr provede Sink test&set a jen
        //   pokud bylo predtim false signalizuje CV.
        // * Mozne ruzne modifikace predchazejiciho, treba pokus se vzbudit jen pokud je ve FIFO vic jak n zaznamu ...
        //
        // Nejlepe udelat misto vycitaciho vlakna vycitaci tasky!
    };
}