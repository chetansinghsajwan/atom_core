export module atom_core:ranges.iterator_concepts;

import std;
import :core;
import :types;

export namespace atom::ranges
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class iterator_tag
    {};

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class unidirectional_iterator_tag: public iterator_tag
    {};

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class bidirectional_iterator_tag: public unidirectional_iterator_tag
    {};

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class random_access_iterator_tag: public bidirectional_iterator_tag
    {};

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class array_iterator_tag: public random_access_iterator_tag
    {};

    template <typename iterator_type>
    class iterator_traits
    {
    public:
        using value_type = typename iterator_type::value_type;
        using tag_type = typename iterator_type::tag_type;
    };

    template <typename in_value_type>
    class iterator_traits<const in_value_type*>
    {
    public:
        using value_type = in_value_type;
        using tag_type = array_iterator_tag;
    };

    template <typename in_value_type>
    class iterator_traits<in_value_type*>
    {
    public:
        using value_type = in_value_type;
        using tag_type = array_iterator_tag;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type>
    using iterator_value_type = typename iterator_traits<iterator_type>::value_type;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type>
    using iterator_tag_type = typename iterator_traits<iterator_type>::tag_type;

    // /// --------------------------------------------------------------------------------------------
    // ///
    // /// --------------------------------------------------------------------------------------------
    // template <typename iterator_type>
    // consteval auto get_iterator_value_type() -> decltype(auto)
    // {}

    // /// --------------------------------------------------------------------------------------------
    // ///
    // /// --------------------------------------------------------------------------------------------
    // template <typename iterator_type>
    // consteval auto get_iterator_tag_type() -> decltype(auto)
    // {}

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename tag_type>
    consteval auto has_iterator_tag() -> bool
    {
        return type_info<iterator_tag_type<iterator_type>>::template is_same_or_derived_from<
            tag_type>();
    }

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
        and has_iterator_tag<iterator_type, iterator_tag>() and std::input_iterator<iterator_type>
        and (type_info<value_type>::is_void()
            or type_info<iterator_value_type<iterator_type>>::template is_same_as<value_type>());
             // or type_info<iterator_type>::template is_dereferencable_to<const value_type&>());

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept const_unidirectional_iterator_concept =
        const_iterator_concept<iterator_type, value_type>
        and has_iterator_tag<iterator_type, unidirectional_iterator_tag>()
        and std::forward_iterator<iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept const_bidirectional_iterator_concept =
        const_unidirectional_iterator_concept<iterator_type, value_type>
        and has_iterator_tag<iterator_type, bidirectional_iterator_tag>()
        and std::bidirectional_iterator<iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept const_random_access_iterator_concept =
        const_bidirectional_iterator_concept<iterator_type, value_type>
        and has_iterator_tag<iterator_type, random_access_iterator_tag>()
        and std::random_access_iterator<iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept const_array_iterator_concept =
        const_random_access_iterator_concept<iterator_type, value_type>
        and has_iterator_tag<iterator_type, array_iterator_tag>()
        and std::contiguous_iterator<iterator_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename value_type = void>
    concept iterator_concept =
        const_iterator_concept<iterator_type, value_type>
        and (type_info<value_type>::is_void()
             or type_info<iterator_value_type<iterator_type>>::template is_same_as<value_type>());
             // or type_info<iterator_type>::template is_dereferencable_to<value_type&>());

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
