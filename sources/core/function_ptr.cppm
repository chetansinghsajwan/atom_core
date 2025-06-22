export module atom_core:core.function_ptr;

namespace atom
{
    template <typename... signature_type>
    struct _function_ptr_impl;

    template <typename result_type, typename... arg_types>
    struct _function_ptr_impl<result_type(arg_types...)>
    {
    public:
        using type = result_type (*)(arg_types...);
    };

    export template <typename... signature_type>
    using function_ptr = typename _function_ptr_impl<signature_type...>::type;
}
