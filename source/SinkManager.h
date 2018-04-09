#pragma once

#include "SinkManagerTemplate.h"
#include "shared_traits.h"

namespace logs2
{
    using SinkManager = SinkManagerTemplate<SharedTraits::AssertCheck>;
}