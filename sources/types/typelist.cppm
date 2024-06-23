export module atom.core:types.typelist;

import std;
import :types.typeutils;
import :types.typeinfo;

namespace atom
{
    using usize = std::size_t;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename... types>
    class typelist;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    namespace _list_ops_type
    {
        template <typename value_type>
        consteval auto _sizeof() -> usize
        {
            if constexpr (typeinfo<value_type>::is_void)
            {
                return 0;
            }
            else
            {
                return sizeof(value_type);
            }
        }

        template <typename value_type>
        consteval auto _get_alignof() -> usize
        {
            if constexpr (typeinfo<value_type>::is_void)
            {
                return 0;
            }
            else
            {
                return alignof(value_type);
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

        template <usize max, typename in_type, typename... types>
        class max_size<max, in_type, types...>
        {
        private:
            static constexpr usize _this_size = _sizeof<in_type>();

        public:
            static constexpr usize value =
                max_size<(_this_size > max ? _this_size : max), types...>::value;
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

        template <usize min, typename in_type, typename... types>
        class min_size<min, in_type, types...>
        {
        private:
            static constexpr usize _this_size = _get_alignof<in_type>();

        public:
            static constexpr usize value =
                min_size<(_this_size < min ? _this_size : min), types...>::value;
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

        template <usize max, typename in_type, typename... types>
        class max_align<max, in_type, types...>
        {
        private:
            static constexpr usize _this_align = _get_alignof<in_type>();

        public:
            static constexpr usize value =
                max_align<(_this_align > max ? _this_align : max), types...>::value;
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

        template <usize min, typename in_type, typename... types>
        class min_align<min, in_type, types...>
        {
        private:
            static constexpr usize _this_align = _sizeof<in_type>();

        public:
            static constexpr usize value =
                min_align<(_this_align < min ? _this_align : min), types...>::value;
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

        template <usize index_to_get, usize index, typename in_type, typename... types>
        class at<index_to_get, index, in_type, types...>
        {
        public:
            using value_type = typeutils::conditional_type<index_to_get == index, in_type,
                typename at<index_to_get, index + 1, types...>::value_type>;
        };

        template <usize index_to_get, usize index>
        class at<index_to_get, index>
        {
        public:
            using value_type = void;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// index_of
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename to_get_type, usize index, typename... types>
        class index_of;

        template <typename to_get_type, usize index, typename in_type, typename... types>
        class index_of<to_get_type, index, in_type, types...>
        {
        public:
            static constexpr usize value = typeinfo<to_get_type>::template is_same_as<in_type>
                                               ? index
                                               : index_of<to_get_type, index + 1, types...>::value;
        };

        template <typename to_get_type, usize index>
        class index_of<to_get_type, index>
        {
        public:
            static constexpr usize value = std::numeric_limits<usize>::max();
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// has
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_type, typename... types>
        class has
        {
        public:
            static constexpr bool value =
                index_of<in_type, 0, types...>::value != std::numeric_limits<usize>::max();
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// add_first
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_type, typename list_type>
        class add_first;

        template <typename in_type, typename... types>
        class add_first<in_type, typelist<types...>>
        {
        public:
            using value_type = typelist<in_type, types...>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// add_last
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_type, typename list_type>
        class add_last;

        template <typename in_type, typename... types>
        class add_last<in_type, typelist<types...>>
        {
        public:
            using value_type = typelist<types..., in_type>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove_if
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <template <typename in_type> typename predicate_type, typename... types>
        class remove_if;

        template <template <typename in_type> typename predicate_type>
        class remove_if<predicate_type>
        {
        public:
            using value_type = typelist<>;
        };

        template <template <typename in_type> typename predicate_type, typename in_type,
            typename... types>
        class remove_if<predicate_type, in_type, types...>
        {
        public:
            using value_type = typeutils::conditional_type<predicate_type<in_type>::value,
                typename add_first<in_type,
                    typename remove_if<predicate_type, types...>::in_type>::value_type,
                typename remove_if<predicate_type, types...>::value_type>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_type, typename... types>
        class remove
        {
            template <typename check_type>
            class _pred
            {
                static constexpr bool value = typeinfo<in_type>::template is_same_as<check_type>;
            };

        public:
            using value_type = typename remove_if<_pred, types...>::value_type;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove_first
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename... ts>
        class remove_first;

        template <typename in_type, typename... types>
        class remove_first<in_type, types...>
        {
        public:
            using value_type = typelist<types...>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove_last
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename... ts>
        class remove_last;

        template <typename in_type, typename... types>
        class remove_last<in_type, types...>
        {
        public:
            using value_type = typename add_first<in_type, remove_last<types...>>::value_type;
        };

        template <typename in_type>
        class remove_last<in_type>
        {
        public:
            using value_type = typelist<>;
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

        template <typename in_type, typename... types>
        class are_unique<in_type, types...>
        {
        public:
            static constexpr bool value =
                not has<in_type, types...>::value and are_unique<types...>::value;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// replace
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename replace_type, typename with_type, typename... types>
        class replace_all;

        template <typename replace_type, typename with_type>
        class replace_all<replace_type, with_type>
        {
        public:
            using value_type = typelist<>;
        };

        template <typename replace_type, typename with_type, typename in_type, typename... types>
        class replace_all<replace_type, with_type, in_type, types...>
        {
            using final_type =
                typeutils::conditional_type<typeinfo<replace_type>::template is_same_as<in_type>,
                    with_type, in_type>;

        public:
            using value_type = typename add_first<final_type,
                typename replace_all<replace_type, with_type, types...>::in_type>::value_type;
        };
    };

    template <typename... types>
    class typelist_impl;

    template <typename type0, typename... types>
    class typelist_impl<type0, types...>
    {
        using this_type = typelist_impl;

    public:
        template <typename function_type>
        static consteval auto for_each(function_type&& func) -> void
        {
            func(typeinfo<type0>());

            typelist_impl<types...>::for_each(forward<function_type>(func));
        }

        template <typename predicate_type>
        static consteval auto are_all(predicate_type&& pred) -> bool
        {
            if (not pred(typeinfo<type0>()))
                return false;

            return typelist_impl<types...>::are_all(forward<predicate_type>(pred));
        }

        template <typename typeinfo_type>
        static consteval auto has(typeinfo_type info) -> bool
        {
            if (typeinfo<type0>::template is_same_as<typename typeinfo_type::value_type>)
                return true;

            return typelist_impl<types...>::has(info);
        }

        template <typename other_type0, typename... other_types>
        static consteval auto has_all(typelist_impl<other_type0, other_types...> list) -> bool
        {
            if (not has(typeinfo<other_type0>()))
                return false;

            if constexpr (sizeof...(other_types) == 0)
                return true;
            else
                return this_type::has_all(typelist_impl<other_types...>());
        }
    };

    template <>
    class typelist_impl<>
    {
    public:
        template <typename function_type>
        static consteval auto for_each(function_type&& func) -> void
        {}

        template <typename predicate_type>
        static consteval auto are_all(predicate_type&& pred) -> bool
        {
            return true;
        }

        template <typename typeinfo_type>
        static consteval auto has(typeinfo_type info) -> bool
        {
            return false;
        }
    };

    export template <typename...>
    class typeinfo_list;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename... types>
    class typelist
    {
        using this_type = typelist;
        using impl_type = typelist_impl<types...>;

    public:
        using info_type = typeinfo_list<this_type>;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize count = _list_ops_type::template count<types...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize max_size = _list_ops_type::template max_size<0, types...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize min_size = _list_ops_type::template max_size<0, types...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize max_align = _list_ops_type::template max_align<0, types...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize min_align = _list_ops_type::template max_align<0, types...>::value;

        /// ----------------------------------------------------------------------------------------
        /// # to do
        /// - try to remove the explicit 0 index.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
            requires(i < count)
        using at = typename _list_ops_type::template at<i, 0, types...>::value_type;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        using front_type = typeutils::conditional_type<count == 0, void,
            typename _list_ops_type::template at<0, 0, types...>::value_type>;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        using back_type = typeutils::conditional_type<count == 0, void,
            typename _list_ops_type::template at<count - 1, 0, types...>::value_type>;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... types_to_check>
        static constexpr bool has =
            (_list_ops_type::template has<types_to_check, types...>::value and ...);

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename to_replace_type, typename with_type>
        using replace_all = typename _list_ops_type::template replace_all<to_replace_type,
            with_type, types...>::value_type;

        /// ----------------------------------------------------------------------------------------
        /// # to do
        /// - try to remove the explicit 0 index.
        /// ----------------------------------------------------------------------------------------
        template <typename in_type>
            requires(has<in_type>)
        static constexpr usize index_of =
            _list_ops_type::template index_of<in_type, 0, types...>::value;

        template <typename function_type>
        static consteval auto for_each(function_type&& func) -> void
        {
            impl_type::for_each(func);
        }

        template <typename predicate_type>
        static consteval auto are_all(predicate_type&& pred) -> bool
        {
            return impl_type::are_all(pred);
        }

        static consteval auto are_unique() -> bool
        {
            return _list_ops_type::template are_unique<types...>::value;
        }

        template <typename typelist_type>
        static consteval auto has_all(const typelist_type& list) -> bool
        {
            return impl_type::has_all(typename typelist_type::impl_type());
        }
    };
}
