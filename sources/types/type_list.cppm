export module atom_core:types.type_list;

import std;
import :core.core;
import :types.type_utils;
import :types.type_info;
import :types.type_list_impl;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename... types>
    class type_list
    {
        using usize = std::size_t;
        using this_type = type_list<types...>;
        using impl_type = type_list_impl<types...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// returns the count of types present. it also counts duplicate types.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_count() -> usize
        {
            return impl_type::get_count();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if count of entries is `0`.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_empty() -> bool
        {
            return impl_type::get_count() == 0;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the size of the type with max size.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_max_size() -> usize
        {
            return impl_type::get_max_size();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the size of the type with min size.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_min_size() -> usize
        {
            return impl_type::get_min_size();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the alignment of the type with max alignment.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_max_align() -> usize
        {
            return impl_type::get_max_align();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the alignment of the type with min alignment.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_min_align() -> usize
        {
            return impl_type::get_min_align();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the `type_info` of type at index `i`.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_at(usize i) -> decltype(auto)
            requires(i < get_count())
        {
            return impl_type::get_at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the `type_info` of type at index `i`, or `void` if index is out of bounds.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_at_try(usize i) -> decltype(auto)
        {
            return impl_type::get_at_try(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the `type_id` of type at index `i`.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto get_id_at(usize i) -> type_id
        {
            return impl_type::get_id_at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the `type_id` of type at index `i`, or null_id if index is out of bounds..
        /// ----------------------------------------------------------------------------------------
        static constexpr auto get_id_at_try(usize i) -> type_id
        {
            return impl_type::get_id_at(i);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize i>
            requires(i < get_count())
        using at_type = typename impl_type::template at_type<i>;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize i>
            requires(i < get_count())
        using at_type_info = type_info<typename impl_type::template at_type<i>>;

        /// ----------------------------------------------------------------------------------------
        /// returns the `type_info` of type at the first.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_first() -> decltype(auto)
            requires(not is_empty())
        {
            return impl_type::get_first();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the `type_info` of type at the first, or `void` if list is empty.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_first_try() -> decltype(auto)
        {
            return impl_type::get_first_try();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the `type_info` of type at the last.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_last() -> decltype(auto)
            requires(not is_empty())
        {
            return impl_type::get_last();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the `type_info` of type at the last, or `void` if list is empty.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_last_try() -> decltype(auto)
        {
            return impl_type::get_last_try();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `i < get_count()`.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto is_index_in_range(usize i) -> bool
        {
            return impl_type::is_index_in_range(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this contains `other_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename other_type>
        static consteval auto has() -> bool
        {
            return impl_type::template has<other_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this contains all of `other_types`.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        static consteval auto has_all() -> bool
        {
            return impl_type::has_all(type_list_impl<other_types...>{});
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this contains all of `other_types`.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        static consteval auto has_all(type_list<other_types...>) -> bool
        {
            return impl_type::has_all(type_list_impl<other_types...>{});
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this contains any of `other_types`.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        static consteval auto has_any() -> bool
        {
            return impl_type::has_any(type_list_impl<other_types...>{});
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this contains any of `other_types`.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        static consteval auto has_any(type_list<other_types...>) -> bool
        {
            return impl_type::has_any(type_list_impl<other_types...>{});
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list after replacing all entries of type `replace_type` with `new_type>`.
        /// ----------------------------------------------------------------------------------------
        template <typename replace_type, typename new_type>
        static consteval auto replace_all() -> decltype(auto)
        {
            auto result = typename impl_type::template replace_all<replace_type, new_type>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list after replacing first entry of type `replace_type` with `new_type>`.
        /// ----------------------------------------------------------------------------------------
        template <typename replace_type, typename with_type>
        static consteval auto replace_first() -> decltype(auto)
        {
            auto result = typename impl_type::template replace_first<replace_type, with_type>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list after replacing last entry of type `replace_type` with `new_type>`.
        /// ----------------------------------------------------------------------------------------
        template <typename replace_type, typename with_type>
        static consteval auto replace_last() -> decltype(auto)
        {
            auto result = typename impl_type::template replace_last<replace_type, with_type>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns hte index of type `other_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename other_type>
        static consteval auto get_index() -> usize
        {
            return impl_type::template get_index<other_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns hte index of type `other_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename other_type>
        static consteval auto get_index(type_info<other_type>) -> usize
        {
            return impl_type::template get_index<other_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes `func` for each type with their `type_info`.
        /// ----------------------------------------------------------------------------------------
        template <typename function_type>
        static constexpr auto for_each(function_type&& func) -> void
        {
            impl_type::for_each(func);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if predicate `pred` returns `true` for all types.
        /// ----------------------------------------------------------------------------------------
        template <typename predicate_type>
        static constexpr auto are_all(predicate_type&& pred) -> bool
        {
            return impl_type::are_all(pred);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if predicate `pred` returns `true` for any type.
        /// ----------------------------------------------------------------------------------------
        template <typename predicate_type>
        static constexpr auto is_any(predicate_type&& pred) -> bool
        {
            return impl_type::is_any(pred);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is present only once.
        /// ----------------------------------------------------------------------------------------
        static consteval auto are_unique() -> bool
        {
            return impl_type::are_unique();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list with `other_type` inserted at index `i`.
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename other_type>
        static consteval auto insert_at() -> decltype(auto)
        {
            auto result = impl_type::template insert_at<i, other_type>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list with `other_type` inserted at index `i`.
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename other_type>
        static consteval auto insert_at(type_info<other_type>) -> decltype(auto)
        {
            auto result = impl_type::template insert_at<i, other_type>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list with `other_type` inserted at first position.
        /// ----------------------------------------------------------------------------------------
        template <typename other_type>
        static consteval auto insert_first() -> type_list<other_type, types...>
        {
            auto result = impl_type::template insert_first<other_type>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list with `other_type` inserted at first position.
        /// ----------------------------------------------------------------------------------------
        template <typename other_type>
        static consteval auto insert_first(type_info<other_type>) -> type_list<other_type, types...>
        {
            auto result = impl_type::template insert_first<other_type>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list with `other_type` inserted at last position.
        /// ----------------------------------------------------------------------------------------
        template <typename other_type>
        static consteval auto insert_last() -> type_list<other_type, types...>
        {
            auto result = impl_type::template insert_last<other_type>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list with `other_type` inserted at first position.
        /// ----------------------------------------------------------------------------------------
        template <typename other_type>
        static consteval auto insert_last(type_info<other_type>) -> type_list<other_type, types...>
        {
            auto result = impl_type::template insert_last<other_type>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list after remove all entries for which `pred` returns `true`.
        /// ----------------------------------------------------------------------------------------
        template <typename predicate_type>
        static consteval auto remove_all_if(predicate_type&& pred) -> decltype(auto)
        {
            auto result = impl_type::remove_all_if(pred);
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list after remove entry at index `i`. if `i` is out of bounds, compiler
        /// error is generated.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        static consteval auto remove_at() -> decltype(auto)
        {
            auto result = impl_type::template remove_at<i>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list after removing first entry.
        /// ----------------------------------------------------------------------------------------
        static consteval auto remove_first() -> decltype(auto)
        {
            auto result = impl_type::remove_first();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list after removing last entry.
        /// ----------------------------------------------------------------------------------------
        static consteval auto remove_last() -> decltype(auto)
        {
            auto result = impl_type::remove_last();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list after removing types other than `other_types`.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        static consteval auto remove_others() -> decltype(auto)
        {
            auto result = impl_type::template remove_others<other_types...>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list after removing types other than `other_types`.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        static consteval auto remove_others(type_list<other_types...>) -> decltype(auto)
        {
            auto result = impl_type::template remove_others<other_types...>();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns new list after removing duplicate entries.
        /// ----------------------------------------------------------------------------------------
        static consteval auto remove_duplicates() -> decltype(auto)
        {
            auto result = impl_type::remove_duplicates();
            return _create_from_impl(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is pure.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_pure() -> bool
        {
            return are_all([](auto info) { return info.is_pure(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is default constructible.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_default_constructible() -> bool
        {
            return are_all([](auto info) { return info.is_default_constructible(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is copy constructible.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_copy_constructible() -> bool
        {
            return are_all([](auto info) { return info.is_copy_constructible(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is copy assignable.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_copy_assignable() -> bool
        {
            return are_all([](auto info) { return info.is_copy_assignable(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is copyable.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_copyable() -> bool
        {
            return are_all([](auto info) { return info.is_copyable(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is move constructible.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_move_constructible() -> bool
        {
            return are_all([](auto info) { return info.is_move_constructible(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is move assignable.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_move_assignable() -> bool
        {
            return are_all([](auto info) { return info.is_move_assignable(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is moveable.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_moveable() -> bool
        {
            return are_all([](auto info) { return info.is_moveable(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is destructible.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_destructible() -> bool
        {
            return are_all([](auto info) { return info.is_destructible(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is destructible or void.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_destructible_or_void() -> bool
        {
            return are_all([](auto info) { return info.is_destructible() or info.is_void(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is trivially default constructible.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_trivially_default_constructible() -> bool
        {
            return are_all([](auto info) { return info.is_trivially_default_constructible(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is trivially copy constructible.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_trivially_copy_constructible() -> bool
        {
            return are_all([](auto info) { return info.is_trivially_copy_constructible(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is triviallycopy assignable.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_trivially_copy_assignable() -> bool
        {
            return are_all([](auto info) { return info.is_trivially_copy_assignable(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is trivially copyable.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_trivially_copyable() -> bool
        {
            return are_all([](auto info) { return info.is_trivially_copyable(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is trivially move constructible.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_trivially_move_constructible() -> bool
        {
            return are_all([](auto info) { return info.is_trivially_move_constructible(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is trivially move assignable.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_trivially_move_assignable() -> bool
        {
            return are_all([](auto info) { return info.is_trivially_move_assignable(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is trivially moveable.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_trivially_moveable() -> bool
        {
            return are_all([](auto info) { return info.is_trivially_moveable(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is trivially destructible.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto are_trivially_destructible() -> bool
        {
            return are_all([](auto info) { return info.is_trivially_destructible(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is convertible to `other_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename other_type>
        static constexpr auto are_convertible_to() -> bool
        {
            return are_all([](auto info) { return info.template is_convertible_to<other_type>(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is equality comparable.
        /// ----------------------------------------------------------------------------------------
        template <typename other_type>
        static constexpr auto are_equality_comparable() -> bool
        {
            return are_all([](auto info) { return info.is_equality_comparable(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if each type is comparable.
        /// ----------------------------------------------------------------------------------------
        template <typename other_type>
        static constexpr auto are_comparable() -> bool
        {
            return are_all([](auto info) { return info.is_comparable(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if all types in this list matches exactly `other_types`.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        static consteval auto is_eq(type_list<other_types...>) -> bool
        {
            return impl_type::template is_eq<other_types...>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result of `is_eq(list)`.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_types>
        constexpr auto operator==(type_list<other_types...> list) const -> bool
        {
            return is_eq(list);
        }

    private:
        template <typename... other_types>
        static consteval auto _create_from_impl(type_list_impl<other_types...>) -> decltype(auto)
        {
            return type_list<other_types...>{};
        }
    };
}
