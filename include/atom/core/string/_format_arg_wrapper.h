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
    template <typename in_value_type>
    class _format_arg_wrapper
    {
        static_assert(typeinfo::is_pure<in_value_type>);

    public:
        using value_type = in_value_type;

    public:
        _format_arg_wrapper(const value_type& value)
            : value(value)
        {}

    public:
        const value_type& value;
    };
}