#pragma once
#include "_string_impl.h"

namespace atom
{
    using string_view = _string_impl<array_view>;
}
