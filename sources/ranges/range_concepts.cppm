export module atom_core:ranges.range_concepts;

import std;
import :types;
import :core;
import :ranges.iterator_concepts;

namespace atom::ranges
{
    template <typename range_type>
    constexpr auto get_iterator(range_type&& range) -> decltype(auto)
        requires requires(range_type&& range) {
            { range.get_iterator() };
        }
    {
        return range.get_iterator();
    }

    template <typename range_type>
    constexpr auto get_iterator_end(range_type&& range) -> decltype(auto)
        requires requires(range_type&& range) {
            { range.get_iterator_end() };
        }
    {
        return range.get_iterator_end();
    }

    template <typename value_type, usize count>
    constexpr auto get_iterator(value_type (&arr)[count]) -> value_type*
    {
        return arr;
    }

    template <typename value_type, usize count>
    constexpr auto get_iterator_end(value_type (&arr)[count]) -> value_type*
    {
        return arr + count;
    }

    template <typename value_type, usize count>
    constexpr auto get_iterator(const value_type (&arr)[count]) -> const value_type*
    {
        return arr;
    }

    template <typename value_type, usize count>
    constexpr auto get_iterator_end(const value_type (&arr)[count]) -> const value_type*
    {
        return arr + count;
    }

    template <typename range_type>
    using const_iterator_type = typename range_type::const_iterator_type;

    template <typename range_type>
    using iterator_type = typename range_type::iterator_type;

    template <typename range_type>
    using iterator_end_type = typename range_type::iterator_end_type;

    template <typename range_type>
    using const_iterator_end_type = typename range_type::const_iterator_end_type;
}

export namespace atom::ranges
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept const_range_concept = requires(const range_type& range) {
        type_info<range_type>::is_pure();
        type_info<value_type>::is_pure();

        { get_iterator(range) };
        { get_iterator_end(range) };

        const_iterator_pair_concept<const_iterator_type<range_type>,
            const_iterator_end_type<range_type>, value_type>;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept range_concept = const_range_concept<range_type, value_type>
                            and iterator_pair_concept<iterator_type<range_type>,
                                iterator_end_type<range_type>, value_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept const_unidirectional_range_concept =
        const_range_concept<range_type, value_type>
        and const_unidirectional_iterator_concept<const_iterator_type<range_type>>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept unidirectional_range_concept =
        range_concept<range_type, value_type>
        and unidirectional_iterator_concept<iterator_type<range_type>>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept const_bidirectional_range_concept =
        const_unidirectional_range_concept<range_type, value_type>
        and const_bidirectional_iterator_concept<const_iterator_type<range_type>>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept bidirectional_range_concept =
        unidirectional_range_concept<range_type, value_type>
        and bidirectional_iterator_concept<iterator_type<range_type>>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept const_random_access_range_concept =
        const_bidirectional_range_concept<range_type, value_type>
        and const_random_access_iterator_concept<const_iterator_type<range_type>>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept random_access_range_concept =
        const_random_access_range_concept<range_type, value_type>
        and bidirectional_range_concept<range_type, value_type>
        and random_access_iterator_concept<iterator_type<range_type>>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept const_array_range_concept =
        const_random_access_range_concept<range_type, value_type>
        and const_array_iterator_concept<const_iterator_type<range_type>>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept array_range_concept = const_array_range_concept<range_type, value_type>
                                  and random_access_range_concept<range_type, value_type>
                                  and array_iterator_concept<iterator_type<range_type>>;
}
