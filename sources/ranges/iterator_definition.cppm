export module atom_core:ranges.iterator_definition;

import :core;
import :types;

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
}
