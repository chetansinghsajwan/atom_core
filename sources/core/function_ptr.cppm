export module atom.core:core.function_ptr;

namespace atom
{
    template <typename... signature_type>
    class _function_ptr_impl;

    template <typename result_t, typename... arg_ts>
    class _function_ptr_impl<result_t(arg_ts...)>
    {
    public:
        using type = result_t (*)(arg_ts...);
    };

    export template <typename... signature_type>
    using function_ptr = typename _function_ptr_impl<signature_type...>::type;
}
