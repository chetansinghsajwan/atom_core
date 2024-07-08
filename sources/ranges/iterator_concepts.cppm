export module atom_core:ranges.iterator_concepts;

import std;
import :core;
import :types;
import :ranges.iterator_definition;

export namespace atom::ranges
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type>
    concept iterator_end_concept =
        type_info<iterator_type>::is_pure() and type_info<iterator_end_type>::is_pure()
        and type_info<iterator_end_type>::is_copyable()
        and type_info<iterator_end_type>::is_moveable()
        and type_info<iterator_end_type>::is_destructible()
        and type_info<iterator_end_type>::template is_equality_comparable_with<iterator_type>();

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept const_iterator_concept =
        type_info<iterator_type>::is_pure() and type_info<value_type>::is_pure()
        and type_info<iterator_definition<iterator_type>>::is_complete()
        and std::input_iterator<iterator_type>
        and enums::has_all_flags(
            iterator_definition<iterator_type>::category, iterator_category::iterator)
        and type_info<typename iterator_definition<iterator_type>::value_type>::is_pure()
        and not type_info<typename iterator_definition<iterator_type>::value_type>::is_void()
        and (type_info<iterator_type>::template is_dereferencable_to<
                 const typename iterator_definition<iterator_type>::value_type&>()
             or type_info<iterator_type>::template is_dereferencable_to<
                 typename iterator_definition<iterator_type>::value_type&>())
        and (type_info<value_type>::is_void()
             or type_info<typename iterator_definition<iterator_type>::value_type>::
                 template is_same_as<value_type>());

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept const_unidirectional_iterator_concept =
        const_iterator_concept<iterator_type, value_type>
        and enums::has_all_flags(iterator_definition<iterator_type>::category,
            iterator_category::unidirectional_iterator)
        and std::forward_iterator<iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept const_bidirectional_iterator_concept =
        const_unidirectional_iterator_concept<iterator_type, value_type>
        and enums::has_all_flags(
            iterator_definition<iterator_type>::category, iterator_category::bidirectional_iterator)
        and std::bidirectional_iterator<iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept const_random_access_iterator_concept =
        const_bidirectional_iterator_concept<iterator_type, value_type>
        and enums::has_all_flags(
            iterator_definition<iterator_type>::category, iterator_category::random_access_iterator)
        and std::random_access_iterator<iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept const_array_iterator_concept =
        const_random_access_iterator_concept<iterator_type, value_type>
        and enums::has_all_flags(
            iterator_definition<iterator_type>::category, iterator_category::array_iterator)
        and std::contiguous_iterator<iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept iterator_concept = const_iterator_concept<iterator_type, value_type>
                               and type_info<iterator_type>::template is_dereferencable_to<
                                   typename iterator_definition<iterator_type>::value_type&>();

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept unidirectional_iterator_concept =
        const_unidirectional_iterator_concept<iterator_type, value_type>
        and iterator_concept<iterator_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept bidirectional_iterator_concept =
        const_bidirectional_iterator_concept<iterator_type, value_type>
        and unidirectional_iterator_concept<iterator_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept random_access_iterator_concept =
        const_random_access_iterator_concept<iterator_type, value_type>
        and bidirectional_iterator_concept<iterator_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept array_iterator_concept = const_array_iterator_concept<iterator_type, value_type>
                                     and random_access_iterator_concept<iterator_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type, typename value_type = void>
    concept const_iterator_pair_concept =
        const_iterator_concept<iterator_type, value_type>
        and iterator_end_concept<iterator_type, iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type, typename value_type = void>
    concept iterator_pair_concept = iterator_concept<iterator_type, value_type>
                                    and iterator_end_concept<iterator_type, iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type, typename value_type = void>
    concept const_unidirectional_iterator_pair_concept =
        const_unidirectional_iterator_concept<iterator_type, value_type>
        and iterator_end_concept<iterator_type, iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type, typename value_type = void>
    concept unidirectional_iterator_pair_concept =
        unidirectional_iterator_concept<iterator_type, value_type>
        and iterator_end_concept<iterator_type, iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type, typename value_type = void>
    concept const_bidirectional_iterator_pair_concept =
        const_bidirectional_iterator_concept<iterator_type, value_type>
        and iterator_end_concept<iterator_type, iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type, typename value_type = void>
    concept bidirectional_iterator_pair_concept =
        bidirectional_iterator_concept<iterator_type, value_type>
        and iterator_end_concept<iterator_type, iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type, typename value_type = void>
    concept const_random_access_iterator_pair_concept =
        const_random_access_iterator_concept<iterator_type, value_type>
        and iterator_end_concept<iterator_type, iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type, typename value_type = void>
    concept random_access_iterator_pair_concept =
        random_access_iterator_concept<iterator_type, value_type>
        and iterator_end_concept<iterator_type, iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type, typename value_type = void>
    concept const_array_iterator_pair_concept =
        const_array_iterator_concept<iterator_type, value_type>
        and iterator_end_concept<iterator_type, iterator_end_type>
        and type_info<iterator_type>::template is_same_as<iterator_end_type>();

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type, typename value_type = void>
    concept array_iterator_pair_concept =
        array_iterator_concept<iterator_type, value_type>
        and iterator_end_concept<iterator_type, iterator_end_type>
        and type_info<iterator_type>::template is_same_as<iterator_end_type>();
}
