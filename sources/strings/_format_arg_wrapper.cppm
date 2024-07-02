export module atom_core:strings._format_arg_wrapper;

import :types;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// this type is used to wrap a type, so that `fmt::formatter` specializations cannot recognize
    /// the type. so that fmt has to use our specialization.
    ///
    /// this is done to take control of the specializations and prevent ambiguity and hide
    /// implementation details.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class _format_arg_wrapper
    {
        static_assert(type_info<value_type>::is_pure());

    public:
        _format_arg_wrapper(const value_type& value)
            : value(value)
        {}

    public:
        const value_type& value;
    };
}
