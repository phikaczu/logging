#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <string>
#include <functional>
#include "circularfifo/circularfifo_memory_sequential_consistent.h"
#include "shared_traits.h"

namespace logs2
{
    //class SinkQueue final
    //{
    //public:
    //    bool empty() const;
    //    template<typename T>
    //    void push(T&& value);
    //    std::string pop();
    //
    //private:
    //    std::queue<std::string> _queue;
    //    mutable std::mutex _queueMtx;
    //};
    //
    //template<typename T>
    //void SinkQueue::push(T&& value)
    //{
    //    std::lock_guard<std::mutex> lk(_queueMtx);
    //    _queue.push(std::forward<T>(value));
    //}

    using SinkQueue = memory_sequential_consistent::CircularFifo<std::string, 25>;

    template <class Check>
    class SinkManagerTemplate final : private Check
    {
    public:
        using Finalizer = std::function<void(std::unique_ptr<SinkQueue>&&)>;

        SinkManagerTemplate();

        // OPEN is this optimization useful? optimization == return a reference if the client wants only to check something?
        //      the whole method is not thread safe, so the argument that the reference could dangle is not valid ...
        const std::shared_ptr<SinkQueue>& getSink() const;
        void setFinalizer(Finalizer&& finalizer);

    private:
        // OPEN is the finalizer needed, maybe as a point of extension ...
        Finalizer _finalizer;
        std::shared_ptr<SinkQueue> _sink;
    };

    template <class Check>
    SinkManagerTemplate<Check>::SinkManagerTemplate()
        : _sink(std::make_shared<SinkQueue>())
    {
    }

    template <class Check>
    const std::shared_ptr<SinkQueue>& SinkManagerTemplate<Check>::getSink() const
    {
        Check::isSameThread();
        return _sink;
    }

    template <class Check>
    void SinkManagerTemplate<Check>::setFinalizer(Finalizer && finalizer)
    {
        _finalizer = finalizer;
    }
}
