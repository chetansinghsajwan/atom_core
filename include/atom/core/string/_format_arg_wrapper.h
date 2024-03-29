#pragma once
#include "atom/core/typeinfo.h"
#include "fmt/core.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// this type is used to wrap a type, so that `fmt::formatter` specializations cannot recognize
    /// the type. so that fmt has to use our specialization.
    ///
    /// this is done to take control of the specializations and prevent ambiguity and hide
    /// implementation details.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class _format_arg_wrapper
    {
        static_assert(typeinfo<value_t>::is_pure);

    public:
        _format_arg_wrapper(const value_t& value)
            : value(value)
        {}

    public:
        const value_t& value;
    };
}
