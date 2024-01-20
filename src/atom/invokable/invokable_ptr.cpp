export module atom.core:invokable_ptr;
import :invokable;

namespace atom
{
    template <typename... tsign>
    class _invokable_ptr_impl;

    template <typename result_type, typename... arg_types>
    class _invokable_ptr_impl<result_type(arg_types...)>
    {
    public:
        using type = result_type (*)(arg_types...);
    };

    export template <typename... tsign>
    using invokable_ptr = typename _invokable_ptr_impl<tsign...>::type;
}
