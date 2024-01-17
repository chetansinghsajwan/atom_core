#pragma once
#include "atom/containers/array_view.h"
#include "_string_impl.h"

namespace atom
{
    template <typename tencoding>
    using basic_string_view = _string_impl<tencoding, array_view<basic_char<tencoding>>>;
}
