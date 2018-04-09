#pragma once

#include "logger_template.h"
#include "shared_traits.h"

namespace logs2
{
    using Logger = LoggerTemplate<SharedTraits::AssertCheck>;
}