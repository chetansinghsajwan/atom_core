export module atom.core:types.type_list_impl;

import std;
import :types.type_utils;
import :types.type_info;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    class type_list_impl;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    struct _remaining_types_helper;

    template <typename type0, typename... types>
    struct _remaining_types_helper<type0, types...>
    {
        using value_type = type_list_impl<types...>;
    };

    template <>
    struct _remaining_types_helper<>
    {
        using value_type = type_list_impl<>;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    struct _type0_helper;

    template <typename type0, typename... types>
    struct _type0_helper<type0, types...>
    {
        using value_type = type0;
    };

    template <>
    struct _type0_helper<>
    {
        using value_type = void;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    class type_list_impl
    {
        template <typename... other_types>
        friend class type_list_impl;

    private:
        using usize = std::size_t;
        using type0 = _type0_helper<types...>::value_type;
        using next_types_list = typename _remaining_types_helper<types...>::value_type;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_count() -> usize
        {
            return sizeof...(types);
        };

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_max_size() -> usize
        {
            if (get_count() == 0)
                return 0;

            usize this_size = type_info<type0>::size;
            usize other_size = next_types_list::get_max_size();

            return this_size > other_size ? this_size : other_size;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_min_size() -> usize
        {
            if (get_count() == 0)
                return 0;

            usize this_size = type_info<type0>::size;
            usize other_size = next_types_list::get_min_size();

            return this_size < other_size ? this_size : other_size;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_max_align() -> usize
        {
            if (get_count() == 0)
                return 0;

            usize this_size = type_info<type0>::size;
            usize other_size = next_types_list::get_max_align();

            return this_size > other_size ? this_size : other_size;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_min_align() -> usize
        {
            if (get_count() == 0)
                return 0;

            usize this_size = type_info<type0>::size;
            usize other_size = next_types_list::get_min_align();

            return this_size < other_size ? this_size : other_size;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        static consteval auto get_at() -> decltype(auto)
        {
            if constexpr (i == 0)
                return type_info<type0>{};
            else
                return next_types_list::template get_at<i - 1>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        static consteval auto get_index() -> usize
        {
            // return 1 so the final result is out of bounds.
            if (get_count() == 0)
                return 1;

            if (type_info<type0>::template is_same_as<value_type>)
                return 0;

            return 1 + next_types_list::template get_index<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        using at_type = typename decltype(get_at<i>())::value_type;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename function_type>
        static consteval auto for_each(function_type&& func) -> void
        {
            if (get_count() == 0)
                return;

            func(type_info<type0>());

            next_types_list::for_each(forward<function_type>(func));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto are_unique() -> bool
        {
            if (get_count() == 0)
                return true;

            if (next_types_list::template has<type0>())
                return false;

            return next_types_list::are_unique();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename predicate_type>
        static consteval auto are_all(predicate_type&& pred) -> bool
        {
            if (get_count() == 0)
                return true;

            if (not pred(type_info<type0>{}))
                return false;

            return next_types_list::are_all(forward<predicate_type>(pred));
        }

        template <typename value_type>
        static consteval auto has() -> bool
        {
            if (get_count() == 0)
                return false;

            if (type_info<type0>::template is_same_as<value_type>)
                return true;

            return next_types_list::template has<value_type>();
        }

        template <typename... other_types>
        static consteval auto has_all(type_list_impl<other_types...> list) -> bool
        {
            using list_type = decltype(list);

            if (list_type::get_count() == 0)
                return true;

            if (get_count() == 0)
                return false;

            if (not has<typename list_type::type0>())
                return false;

            return has_all(typename list_type::next_types_list{});
        }

        template <typename other_type0, typename... other_types>
        static consteval auto has_any(type_list_impl<other_type0, other_types...> list) -> bool
        {
            if (get_count() == 0)
                return false;

            if (has<other_type0>())
                return true;

            return has_any(type_list_impl<other_types...>{});
        }

        template <typename function_type>
        static consteval auto has_if(function_type&& func) -> bool
        {
            if (get_count() == 0)
                return false;

            if (func(type_info<type0>{}))
                return true;

            return next_types_list::has_if(forward<function_type>(func));
        }

        // /// ----------------------------------------------------------------------------------------
        // ///
        // /// ----------------------------------------------------------------------------------------
        // template <usize index_to_get, usize index>
        // class at;

        // template <usize index_to_get, usize index, typename in_type>
        // class at<index_to_get, index, in_type, types...>
        // {
        // public:
        //     using value_type = type_utils::conditional_type<index_to_get == index, in_type,
        //         typename at<index_to_get, index + 1, types...>::value_type>;
        // };

        // template <usize index_to_get, usize index>
        // class at<index_to_get, index>
        // {
        // public:
        //     using value_type = void;
        // };

        // /// ----------------------------------------------------------------------------------------
        // ///
        // /// ----------------------------------------------------------------------------------------
        // template <typename type, usize index, typename... types>
        // class index_of;

        // template <typename type, usize index, typename in_type, typename... types>
        // class index_of<type, index, in_type, types...>
        // {
        // public:
        //     static constexpr usize value = type_info<type>::template is_same_as<in_type>
        //                                        ? index
        //                                        : index_of<type, index + 1, types...>::value;
        // };

        // template <typename type, usize index>
        // class index_of<type, index>
        // {
        // public:
        //     static constexpr usize value = std::numeric_limits<usize>::max();
        // };

        // ////////////////////////////////////////////////////////////////////////////////////////////
        // ////
        // //// has
        // ////
        // ////////////////////////////////////////////////////////////////////////////////////////////

        // template <typename in_type, typename... types>
        // class has
        // {
        // public:
        //     static constexpr bool value =
        //         index_of<in_type, 0, types...>::value != std::numeric_limits<usize>::max();
        // };

        // ////////////////////////////////////////////////////////////////////////////////////////////
        // ////
        // //// add_first
        // ////
        // ////////////////////////////////////////////////////////////////////////////////////////////

        // template <typename in_type, typename list_type>
        // class add_first;

        // template <typename in_type, typename... types>
        // class add_first<in_type, type_list<types...>>
        // {
        // public:
        //     using value_type = type_list<in_type, types...>;
        // };

        // ////////////////////////////////////////////////////////////////////////////////////////////
        // ////
        // //// add_last
        // ////
        // ////////////////////////////////////////////////////////////////////////////////////////////

        // template <typename in_type, typename list_type>
        // class add_last;

        // template <typename in_type, typename... types>
        // class add_last<in_type, type_list<types...>>
        // {
        // public:
        //     using value_type = type_list<types..., in_type>;
        // };

        // ////////////////////////////////////////////////////////////////////////////////////////////
        // ////
        // //// remove_if
        // ////
        // ////////////////////////////////////////////////////////////////////////////////////////////

        // template <template <typename in_type> typename predicate_type, typename... types>
        // class remove_if;

        // template <template <typename in_type> typename predicate_type>
        // class remove_if<predicate_type>
        // {
        // public:
        //     using value_type = type_list<>;
        // };

        // template <template <typename in_type> typename predicate_type, typename in_type,
        //     typename... types>
        // class remove_if<predicate_type, in_type, types...>
        // {
        // public:
        //     using value_type = type_utils::conditional_type<predicate_type<in_type>::value,
        //         typename add_first<in_type,
        //             typename remove_if<predicate_type, types...>::in_type>::value_type,
        //         typename remove_if<predicate_type, types...>::value_type>;
        // };

        // ////////////////////////////////////////////////////////////////////////////////////////////
        // ////
        // //// remove
        // ////
        // ////////////////////////////////////////////////////////////////////////////////////////////

        // template <typename in_type, typename... types>
        // class remove
        // {
        //     template <typename check_type>
        //     class _pred
        //     {
        //         static constexpr bool value = type_info<in_type>::template is_same_as<check_type>;
        //     };

        // public:
        //     using value_type = typename remove_if<_pred, types...>::value_type;
        // };

        // ////////////////////////////////////////////////////////////////////////////////////////////
        // ////
        // //// remove_first
        // ////
        // ////////////////////////////////////////////////////////////////////////////////////////////

        // template <typename... types>
        // class remove_first;

        // template <typename in_type, typename... types>
        // class remove_first<in_type, types...>
        // {
        // public:
        //     using value_type = type_list<types...>;
        // };

        // ////////////////////////////////////////////////////////////////////////////////////////////
        // ////
        // //// remove_last
        // ////
        // ////////////////////////////////////////////////////////////////////////////////////////////

        // template <typename... types>
        // class remove_last;

        // template <typename in_type, typename... types>
        // class remove_last<in_type, types...>
        // {
        // public:
        //     using value_type = typename add_first<in_type, remove_last<types...>>::value_type;
        // };

        // template <typename in_type>
        // class remove_last<in_type>
        // {
        // public:
        //     using value_type = type_list<>;
        // };

        // ////////////////////////////////////////////////////////////////////////////////////////////
        // ////
        // //// replace
        // ////
        // ////////////////////////////////////////////////////////////////////////////////////////////

        // template <typename replace_type, typename with_type, typename... types>
        // class replace_all;

        // template <typename replace_type, typename with_type>
        // class replace_all<replace_type, with_type>
        // {
        // public:
        //     using value_type = type_list<>;
        // };

        // template <typename replace_type, typename with_type, typename in_type, typename... types>
        // class replace_all<replace_type, with_type, in_type, types...>
        // {
        //     using final_type =
        //         type_utils::conditional_type<type_info<replace_type>::template is_same_as<in_type>,
        //             with_type, in_type>;

        // public:
        //     using value_type = typename add_first<final_type,
        //         typename replace_all<replace_type, with_type, types...>::in_type>::value_type;
        // };
    };
}
