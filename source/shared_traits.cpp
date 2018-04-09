#include "shared_traits.h"

#include<cassert>

using namespace logs2::SharedTraits;

AssertCheck::AssertCheck()
    : _threadId{ std::this_thread::get_id() }
{
}

void AssertCheck::isSameThread() const
{
    assert(std::this_thread::get_id() == _threadId);
}
