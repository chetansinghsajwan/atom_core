export module atom.core:string_functions;
import :core;

namespace atom
{
    template <template <typename value_type> typename container_type>
    class string_functions: public container_type<uchar>
    {
        using this_type = string_functions<container_type>;
        using base_type = container_type<uchar>;

    public:
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;
        // using mut_iter_type = typename base_type::mut_iter_type;
        // using mut_iter_end_type = typename base_type::mut_iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize::unwrapped_type count>
        constexpr string_functions(const char (&arr)[count])
            : base_type(make_range(arr))
        {}

    public:
        constexpr auto to_std_char_ptr() const -> const char*
        {
            return _to_std_char_ptr(this->data().unwrap());
        }

        constexpr auto to_std_char_ptr() -> char*
        {
            return _to_std_char_ptr(this->mut_data().unwrap());
        }
    };
}
