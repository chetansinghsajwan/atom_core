export module atom.core:invokable;
import :std;

namespace atom
{
    template <typename invokable_type, typename... tsignature>
    class _is_invokable_impl;

    template <typename invokable_type, typename result_type, typename... arg_types>
    class _is_invokable_impl<invokable_type, result_type(arg_types...)>
    {
    public:
        static constexpr bool value = std::is_invocable_r_v<result_type, invokable_type, arg_types...>;
    };

    /// @todo add impl for const invocable.
    template <typename invokable_type, typename result_type, typename... arg_types>
    class _is_invokable_impl<invokable_type, result_type(arg_types...) const>
    {
    public:
        static constexpr bool value = std::is_invocable_r_v<result_type, invokable_type, arg_types...>;
    };

    export template <typename invokable_type, typename... tsignature>
    concept rinvokable = _is_invokable_impl<invokable_type, tsignature...>::value;
}
