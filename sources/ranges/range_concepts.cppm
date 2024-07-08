export module atom_core:ranges.range_concepts;

import std;
import :types;
import :core;
import :ranges.iterator_concepts;
import :ranges.range_definition;

export namespace atom::ranges
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept const_range_concept = requires(const range_type& range) {
        requires type_info<range_type>::is_pure();
        requires type_info<value_type>::is_pure();
        requires type_info<range_definition<range_type>>::is_complete();

        typename range_definition<range_type>::value_type;
        typename range_definition<range_type>::const_iterator_type;
        typename range_definition<range_type>::const_iterator_end_type;

        {
            range_definition<range_type>::get_const_iterator(range)
        } -> std::same_as<typename range_definition<range_type>::const_iterator_type>;

        {
            range_definition<range_type>::get_const_iterator_end(range)
        } -> std::same_as<typename range_definition<range_type>::const_iterator_end_type>;

        const_iterator_pair_concept<typename range_definition<range_type>::const_iterator_type,
            typename range_definition<range_type>::const_iterator_end_type, value_type>;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept range_concept = requires(range_type& range) {
        requires const_range_concept<range_type, value_type>;

        typename range_definition<range_type>::iterator_type;
        typename range_definition<range_type>::iterator_end_type;

        {
            range_definition<range_type>::get_iterator(range)
        } -> std::same_as<typename range_definition<range_type>::iterator_type>;

        {
            range_definition<range_type>::get_iterator_end(range)
        } -> std::same_as<typename range_definition<range_type>::iterator_end_type>;

        iterator_pair_concept<typename range_definition<range_type>::iterator_type,
            typename range_definition<range_type>::iterator_end_type, value_type>;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept const_unidirectional_range_concept =
        const_range_concept<range_type, value_type>
        and const_unidirectional_iterator_concept<
            typename range_definition<range_type>::const_iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept unidirectional_range_concept =
        range_concept<range_type, value_type>
        and unidirectional_iterator_concept<typename range_definition<range_type>::iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept const_bidirectional_range_concept =
        const_unidirectional_range_concept<range_type, value_type>
        and const_bidirectional_iterator_concept<
            typename range_definition<range_type>::const_iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept bidirectional_range_concept =
        unidirectional_range_concept<range_type, value_type>
        and bidirectional_iterator_concept<typename range_definition<range_type>::iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept const_random_access_range_concept =
        const_bidirectional_range_concept<range_type, value_type>
        and const_random_access_iterator_concept<
            typename range_definition<range_type>::const_iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept random_access_range_concept =
        const_random_access_range_concept<range_type, value_type>
        and bidirectional_range_concept<range_type, value_type>
        and random_access_iterator_concept<typename range_definition<range_type>::iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept const_array_range_concept =
        const_random_access_range_concept<range_type, value_type>
        and const_array_iterator_concept<
            typename range_definition<range_type>::const_iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type = void>
    concept array_range_concept =
        const_array_range_concept<range_type, value_type>
        and random_access_range_concept<range_type, value_type>
        and array_iterator_concept<typename range_definition<range_type>::iterator_type>;
}
