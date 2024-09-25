export module atom_core:types.type_list_impl;

import std;
import :core.core;
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
        using this_type = type_list_impl<types...>;
        using usize = std::size_t;
        using type0 = typename _type0_helper<types...>::value_type;
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

            usize this_size = type_info<type0>::get_size();
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

            usize this_size = type_info<type0>::get_size();
            usize other_size = next_types_list::get_min_size();

            if (next_types_list::get_count() == 0)
                return this_size;

            return this_size < other_size ? this_size : other_size;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_max_align() -> usize
        {
            if (get_count() == 0)
                return 0;

            usize this_size = type_info<type0>::get_align();
            usize other_size = next_types_list::get_max_align();

            if (next_types_list::get_count() == 0)
                return this_size;

            return this_size > other_size ? this_size : other_size;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_min_align() -> usize
        {
            if (get_count() == 0)
                return 0;

            usize this_size = type_info<type0>::get_align();
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
        static constexpr auto get_id_at(usize i) -> type_id
        {
            if (i == 0)
                return type_info<type0>::get_id();

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

            if (type_info<type0>::template is_same_as<value_type>())
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
        static constexpr auto for_each(function_type&& func) -> void
        {
            if constexpr (get_count() == 0)
                return;
            else
            {
                loop_command command = loop_command::continue_;
                func(&command, type_info<type0>());
                if (command == loop_command::break_)
                    return;

                next_types_list::for_each(forward<function_type>(func));
            }
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
        static constexpr auto are_all(predicate_type&& pred) -> bool
        {
            if (get_count() == 0)
                return true;

            if (not pred(type_info<type0>{}))
                return false;

            return next_types_list::are_all(forward<predicate_type>(pred));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename predicate_type>
        static constexpr auto is_any(predicate_type&& pred) -> bool
        {
            if (get_count() == 0)
                return false;

            if (pred(type_info<type0>{}))
                return true;

            return next_types_list::is_any(forward<predicate_type>(pred));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr auto is_index_in_range(usize i) -> bool
        {
            return i < get_count();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        static consteval auto has() -> bool
        {
            if (get_count() == 0)
                return false;

            if (type_info<type0>::template is_same_as<value_type>())
                return true;

            return next_types_list::template has<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
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

        template <typename... other_types>
        static consteval auto has_any(type_list_impl<other_types...> list) -> bool
        {
            using that_list_type = decltype(list);

            if (that_list_type::get_count() == 0)
                return true;

            if (get_count() == 0)
                return false;

            if (has<typename that_list_type::type0>())
                return true;

            return has_any(typename that_list_type::next_types_list{});
        }

        template <typename function_type>
        static constexpr auto has_if(function_type&& func) -> bool
        {
            if (get_count() == 0)
                return false;

            if (func(type_info<type0>{}))
                return true;

            return next_types_list::has_if(forward<function_type>(func));
        }

        template <usize i, typename other_type>
        static consteval auto insert_at() -> decltype(auto)
        {
            constexpr auto inserter = [&]<usize current_index = 0>(
                                          this auto self, auto current_list, auto result_list)
            {
                using result_list_type = decltype(result_list);
                using current_list_type = decltype(current_list);
                using next_list_type = typename current_list_type::next_types_list;
                using current_type = typename current_list_type::type0;

                if constexpr (current_list_type::get_count() == 0)
                    return result_list;
                else if constexpr (current_index == i)
                {
                    return self.template operator()<current_index + 1>(
                        next_list_type{}, result_list.template insert_last<other_type>()
                                              .template insert_last<current_type>());
                }
                else
                {
                    return self.template operator()<current_index + 1>(
                        next_list_type{}, result_list.template insert_last<current_type>());
                }
            };

            return inserter(this_type{}, type_list_impl<>{});
        }

        template <typename other_type>
        static consteval auto insert_first() -> type_list_impl<other_type, types...>
        {
            return {};
        }

        template <typename other_type>
        static consteval auto insert_last() -> type_list_impl<types..., other_type>
        {
            return {};
        }

        template <typename predicate_type>
        static consteval auto remove_all_if(predicate_type&& pred) -> decltype(auto)
        {
            consteval_function consteval_pred{ pred };

            // we remove entries by iterating the original list and adding only those entries to
            // a new list which should not be removed.
            auto remover = [&](this auto self, auto current_list, auto result_list)
            {
                using result_list_type = decltype(result_list);
                using current_list_type = decltype(current_list);
                using next_list_type = typename current_list_type::next_types_list;
                using current_type = typename current_list_type::type0;

                if constexpr (current_list_type::get_count() == 0)
                    return result_list;
                else if constexpr (consteval_pred(type_info<current_type>{}))
                    return self(next_list_type{}, result_list);
                else
                    return self(
                        next_list_type{}, result_list_type::template insert_last<current_type>());
            };

            return remover(this_type{}, type_list_impl<>{});
        }

        template <usize i>
        static consteval auto remove_at() -> decltype(auto)
        {
            // constexpr usize index = 0;
            constexpr auto pred = [j = 0](auto type) mutable { return i == j++; };

            return remove_all_if<pred>();
        }

        static consteval auto remove_first() -> decltype(auto)
        {
            return remove_at<0>();
        }

        static consteval auto remove_last() -> decltype(auto)
        {
            return remove_at<get_count() - 1>();
        }

        template <typename... other_types>
        static consteval auto remove_others() -> decltype(auto)
        {
            auto pred = [](auto info)
            {
                using value_type = typename decltype(info)::value_type;
                return not type_list_impl<other_types...>::template has<value_type>();
            };

            return remove_all_if(pred);
        }

        template <typename... other_types>
        static consteval auto is_eq() -> bool
        {
            using that_list = type_list_impl<other_types...>;
            return type_info<this_type>::template is_same_as<that_list>();
        }
    };
}
