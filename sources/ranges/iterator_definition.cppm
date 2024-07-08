export module atom_core:ranges.iterator_definition;

import std;
import :core;
import :types;
import :contracts;

export namespace atom::ranges
{
    enum class iterator_category : byte
    {
        iterator = 0b1,
        unidirectional_iterator = 0b11,
        bidirectional_iterator = 0b111,
        random_access_iterator = 0b1111,
        array_iterator = 0b11111,
    };

    template <typename iterator_type>
    class iterator_definition;

    template <typename in_value_type>
    class iterator_definition<const in_value_type*>
    {
    public:
        using value_type = in_value_type;
        static constexpr iterator_category category = iterator_category::array_iterator;
    };

    template <typename in_value_type>
    class iterator_definition<in_value_type*>
    {
    public:
        using value_type = in_value_type;
        static constexpr iterator_category category = iterator_category::array_iterator;
    };

    template <typename iterator_type>
        requires(std::input_iterator<iterator_type>)
    class iterator_definition<iterator_type>
    {
        using std_traits_type = std::iterator_traits<iterator_type>;

        static constexpr auto _get_category() -> iterator_category
        {
            if constexpr (std::contiguous_iterator<iterator_type>)
                return ranges::iterator_category::array_iterator;

            if constexpr (std::random_access_iterator<iterator_type>)
                return ranges::iterator_category::random_access_iterator;

            if constexpr (std::bidirectional_iterator<iterator_type>)
                return ranges::iterator_category::bidirectional_iterator;

            if constexpr (std::forward_iterator<iterator_type>)
                return ranges::iterator_category::unidirectional_iterator;

            return ranges::iterator_category::iterator;
        }

    public:
        using value_type = typename std_traits_type::value_type;
        static constexpr iterator_category category = _get_category();
    };
}
