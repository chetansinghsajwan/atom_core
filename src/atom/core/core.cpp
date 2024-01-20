export module atom.core:core.core;
import :std;

export namespace atom
{
    template <typename value_type>
    constexpr auto mov(value_type&& val) -> typename std::remove_reference_t<value_type>&&
    {
        return static_cast<typename std::remove_reference_t<value_type>&&>(val);
    }

    template <typename value_type>
    constexpr auto forward(typename std::remove_reference_t<value_type>& val) -> value_type&&
    {
        return static_cast<value_type&&>(val);
    }

    template <typename value_type>
    constexpr auto forward(typename std::remove_reference_t<value_type>&& val) -> value_type&&
    {
        static_assert(
            !std::is_lvalue_reference_v<value_type>, "can not forward an rvalue as an lvalue.");

        return static_cast<value_type&&>(val);
    }

    template <typename value_type>
    using pure_type = std::remove_cvref_t<value_type>;
}
