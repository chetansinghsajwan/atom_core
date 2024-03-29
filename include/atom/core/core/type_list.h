#pragma once
#include "atom/core/core.h"
#include "atom/core/typeinfo.h"
#include "atom/core/math.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    class type_list;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    namespace _list_ops_t
    {
        template <typename value_t>
        consteval auto _sizeof() -> usize
        {
            if constexpr (typeinfo<value_t>::is_void)
            {
                return 0;
            }
            else
            {
                return sizeof(value_t);
            }
        }

        template <typename value_t>
        consteval auto _get_alignof() -> usize
        {
            if constexpr (typeinfo<value_t>::is_void)
            {
                return 0;
            }
            else
            {
                return alignof(value_t);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// count
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename... ts>
        class count
        {
        public:
            static constexpr usize value = sizeof...(types);
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// max_size
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <usize max, typename... ts>
        class max_size;

        template <usize max, typename in_t, typename... types>
        class max_size<max, in_t, types...>
        {
        private:
            static constexpr usize _this_size = _sizeof<in_t>();

        public:
            static constexpr usize value =
                max_size<(_this_size > max ? _this_size : max), ts...>::value;
        };

        template <usize max>
        class max_size<max>
        {
        private:
            static constexpr usize _this_size = 0;

        public:
            static constexpr usize value = _this_size > max ? _this_size : max;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// min_size
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <usize min, typename... ts>
        class min_size;

        template <usize min, typename in_t, typename... types>
        class min_size<min, in_t, types...>
        {
        private:
            static constexpr usize _this_size = _get_alignof<in_t>();

        public:
            static constexpr usize value =
                min_size<(_this_size < min ? _this_size : min), ts...>::value;
        };

        template <usize min>
        class min_size<min>
        {
        private:
            static constexpr usize _this_size = 0;

        public:
            static constexpr usize value = _this_size < min ? _this_size : min;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// max_align
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <usize max, typename... ts>
        class max_align;

        template <usize max, typename in_t, typename... types>
        class max_align<max, in_t, types...>
        {
        private:
            static constexpr usize _this_align = _get_alignof<in_t>();

        public:
            static constexpr usize value =
                max_align<(_this_align > max ? _this_align : max), ts...>::value;
        };

        template <usize max>
        class max_align<max>
        {
        private:
            static constexpr usize _this_align = 0;

        public:
            static constexpr usize value = _this_align > max ? _this_align : max;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// min_align
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <usize min, typename... ts>
        class min_align;

        template <usize min, typename in_t, typename... types>
        class min_align<min, in_t, types...>
        {
        private:
            static constexpr usize _this_align = _sizeof<in_t>();

        public:
            static constexpr usize value =
                min_align<(_this_align < min ? _this_align : min), ts...>::value;
        };

        template <usize min>
        class min_align<min>
        {
        private:
            static constexpr usize _this_align = 0;

        public:
            static constexpr usize value = _this_align < min ? _this_align : min;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// at
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <usize index_to_get, usize index, typename... ts>
        class at;

        template <usize index_to_get, usize index, typename in_t, typename... types>
        class at<index_to_get, index, in_t, types...>
        {
        public:
            using value_t = typeutils::conditional_t<index_to_get == index, in_t,
                typename at<index_to_get, index + 1, types...>::value_t>;
        };

        template <usize index_to_get, usize index>
        class at<index_to_get, index>
        {
        public:
            using value_t = void;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// index_of
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename to_get_t, usize index, typename... types>
        class index_of;

        template <typename to_get_t, usize index, typename in_t, typename... types>
        class index_of<to_get_t, index, in_t, types...>
        {
        public:
            static constexpr usize value =
                is_same_as<to_get_t, in_t> ? index : index_of<to_get_t, index + 1, types...>::value;
        };

        template <typename to_get_t, usize index>
        class index_of<to_get_t, index>
        {
        public:
            static constexpr usize value = math::max<usize>();
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// has
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_t, typename... types>
        class has
        {
        public:
            static constexpr bool value = index_of<in_t, 0, types...>::value != math::max<usize>();
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// add_first
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_t, typename list_t>
        class add_first;

        template <typename in_t, typename... types>
        class add_first<in_t, type_list<types...>>
        {
        public:
            using value_t = type_list<in_t, types...>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// add_last
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_t, typename list_t>
        class add_last;

        template <typename in_t, typename... types>
        class add_last<in_t, type_list<types...>>
        {
        public:
            using value_t = type_list<types..., in_t>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove_if
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <template <typename in_t> typename predicate_t, typename... types>
        class remove_if;

        template <template <typename in_t> typename predicate_t>
        class remove_if<predicate_t>
        {
        public:
            using value_t = type_list<>;
        };

        template <template <typename in_t> typename predicate_t, typename in_t, typename... types>
        class remove_if<predicate_t, in_t, types...>
        {
        public:
            using value_t = typeutils::conditional_t<predicate_t<in_t>::value,
                typename add_first<in_t, typename remove_if<predicate_t, types...>::in_t>::value_t,
                typename remove_if<predicate_t, types...>::value_t>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_t, typename... types>
        class remove
        {
            template <typename check_t>
            class _pred
            {
                static constexpr bool value = is_same_as<in_t, check_t>;
            };

        public:
            using value_t = typename remove_if<_pred, types...>::value_t;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove_first
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename... ts>
        class remove_first;

        template <typename in_t, typename... types>
        class remove_first<in_t, types...>
        {
        public:
            using value_t = type_list<types...>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove_last
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename... ts>
        class remove_last;

        template <typename in_t, typename... types>
        class remove_last<in_t, types...>
        {
        public:
            using value_t = typename add_first<in_t, remove_last<types...>>::value_t;
        };

        template <typename in_t>
        class remove_last<in_t>
        {
        public:
            using value_t = type_list<>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// are_unique
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename... ts>
        class are_unique;

        template <>
        class are_unique<>
        {
        public:
            static constexpr bool value = true;
        };

        template <typename in_t, typename... types>
        class are_unique<in_t, types...>
        {
        public:
            static constexpr bool value =
                not has<in_t, types...>::value and are_unique<types...>::value;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// replace
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename replace_t, typename with_t, typename... types>
        class replace_all;

        template <typename replace_t, typename with_t>
        class replace_all<replace_t, with_t>
        {
        public:
            using value_t = type_list<>;
        };

        template <typename replace_t, typename with_t, typename in_t, typename... types>
        class replace_all<replace_t, with_t, in_t, types...>
        {
            using final_t =
                typeutils::conditional_t<typeinfo<replace_t>::template is_same_as<in_t>, with_t, in_t>;

        public:
            using value_t = typename add_first<final_t,
                typename replace_all<replace_t, with_t, types...>::in_t>::value_t;
        };
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    class type_list
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize count = _list_ops_t::template count<types...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize max_size = _list_ops_t::template max_size<0, types...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize min_size = _list_ops_t::template max_size<0, types...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize max_align = _list_ops_t::template max_align<0, types...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize min_align = _list_ops_t::template max_align<0, types...>::value;

        /// ----------------------------------------------------------------------------------------
        /// # to do
        /// - try to remove the explicit 0 index.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
            requires(i < count)
        using at = typename _list_ops_t::template at<i, 0, types...>::value_t;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... ts_to_check>
        static constexpr bool has =
            (_list_ops_t::template has<ts_to_check, types...>::value and ...);

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename to_replace_t, typename with_t>
        using replace_all =
            typename _list_ops_t::template replace_all<to_replace_t, with_t, types...>::value_t;

        /// ----------------------------------------------------------------------------------------
        /// # to do
        /// - try to remove the explicit 0 index.
        /// ----------------------------------------------------------------------------------------
        template <typename in_t>
            requires(has<in_t>)
        static constexpr usize index_of = _list_ops_t::template index_of<in_t, 0, types...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr bool are_unique = _list_ops_t::template are_unique<types...>::value;
    };
}
