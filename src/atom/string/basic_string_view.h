#pragma once
#include "atom/containers/array_view.h"
#include "_string_impl.h"

namespace atom
{
    template <typename encoding_type>
    using basic_string_view = _string_impl<encoding_type, array_view<basic_char<encoding_type>>>;
}
