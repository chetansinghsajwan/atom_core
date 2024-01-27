export module atom.core:result;
import :variant;

export namespace atom
{
    template <typename result_type, typename... error_types>
    class result: public variant<result_type, error_types...>
    {
        using base_type = variant<result_type, error_types...>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class _result_void
    {};

    template <typename... error_types>
    class result<void, error_types...>: public variant<_result_void, error_types...>
    {
        using base_type = variant<_result_void, error_types...>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    template <typename... error_types>
    constexpr auto success() -> result<void, error_types...>
    {
        using result_type = result<void, error_types...>;
        return result_type(_result_void());
    }
}
