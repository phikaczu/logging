#include "LogSink.h"

#include <cassert>
#include "SinkManager.h"

using namespace logs2;

// TODO store some vector of appenders, so Sink::initSink will work for any number ;)
static thread_local bool initialized{ false };
static thread_local SinkManager sinkMgr;

void Sink::initSink(Appender& appender)
{
    if (initialized) return;

    appender.registerSinkManager(sinkMgr);
    initialized = true;
}

void Sink::log(const std::string& message)
{
    assert(initialized);
    // TODO signal error if the push is not successful
    (void)sinkMgr.getSink()->push(message);
}

void Sink::log(std::string&& message)
{
    assert(initialized);
    // TODO signal error if the push is not successful
    (void)sinkMgr.getSink()->push(std::move(message));
}