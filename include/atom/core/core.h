#pragma once
#include "atom/core/core/core.h"
#include "atom/core/core/bitset.h"
#include "atom/core/core/byte.h"
#include "atom/core/core/char_wrapper.h"
#include "atom/core/core/int_wrapper.h"
#include "atom/core/core/float_wrapper.h"
#include "atom/core/core/requirements.h"

#include "atom/core/_std.h"

namespace atom
{
    using nullptr_t = std::nullptr_t;

    template <typename... arg_ts>
    class type_holder
    {};

    template <typename value_t>
    using initializer_list = std::initializer_list<value_t>;
}
