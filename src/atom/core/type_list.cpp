export module atom.core:type_list;
import :core;
import :tti;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename... types>
    class type_list;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    namespace _type_list_ops
    {
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

        template <usize max, typename in_type, typename... ts>
        class max_size<max, in_type, ts...>
        {
        private:
            static constexpr usize _this_size = _sizeof<in_type>();

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

        template <usize min, typename in_type, typename... ts>
        class min_size<min, in_type, ts...>
        {
        private:
            static constexpr usize _this_size = _alignof<in_type>();

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

        template <usize max, typename in_type, typename... ts>
        class max_align<max, in_type, ts...>
        {
        private:
            static constexpr usize _this_align = _alignof<in_type>();

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

        template <usize min, typename in_type, typename... ts>
        class min_align<min, in_type, ts...>
        {
        private:
            static constexpr usize _this_align = _sizeof<in_type>();

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

        template <usize index_to_get, usize index, typename in_type, typename... ts>
        class at<index_to_get, index, in_type, ts...>
        {
        public:
            using type = tti::conditional_type<index_to_get == index, in_type,
                typename at<index_to_get, index + 1, types...>::type>;
        };

        template <usize index_to_get, usize index>
        class at<index_to_get, index>
        {
        public:
            using type = void;
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
            static constexpr usize value = rsame_as<to_get_type, in_type>
                                               ? index
                                               : index_of<to_get_type, index + 1, types...>::value;
        };

        template <typename to_get_type, usize index>
        class index_of<to_get_type, index>
        {
        public:
            static constexpr usize value = usize::max();
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// has
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_type, typename... ts>
        class has
        {
        public:
            static constexpr bool value = index_of<in_type, 0, types...>::value != usize::max();
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// add_first
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_type, typename tlist>
        class add_first;

        template <typename in_type, typename... ts>
        class add_first<in_type, type_list<ts...>>
        {
        public:
            using type = type_list<in_type, ts...>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// add_last
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_type, typename tlist>
        class add_last;

        template <typename in_type, typename... ts>
        class add_last<in_type, type_list<ts...>>
        {
        public:
            using type = type_list<ts..., in_type>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove_if
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <template <typename in_type> typename predicate_type, typename... ts>
        class remove_if;

        template <template <typename in_type> typename predicate_type>
        class remove_if<predicate_type>
        {
        public:
            using type = type_list<>;
        };

        template <template <typename in_type> typename predicate_type, typename in_type,
            typename... ts>
        class remove_if<predicate_type, in_type, ts...>
        {
        public:
            using type = tti::conditional_type<predicate_type<in_type>::value,
                typename add_first<in_type,
                    typename remove_if<predicate_type, ts...>::in_type>::type,
                typename remove_if<predicate_type, ts...>::type>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename in_type, typename... ts>
        class remove
        {
            template <typename check_type>
            class _pred
            {
                static constexpr bool value = rsame_as<in_type, check_type>;
            };

        public:
            using type = typename remove_if<_pred, ts...>::type;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove_first
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename... ts>
        class remove_first;

        template <typename in_type, typename... ts>
        class remove_first<in_type, ts...>
        {
        public:
            using type = type_list<ts...>;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// remove_last
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename... ts>
        class remove_last;

        template <typename in_type, typename... ts>
        class remove_last<in_type, ts...>
        {
        public:
            using type = typename add_first<in_type, remove_last<ts...>>::type;
        };

        template <typename in_type>
        class remove_last<in_type>
        {
        public:
            using type = type_list<>;
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

        template <typename in_type, typename... ts>
        class are_unique<in_type, ts...>
        {
        public:
            static constexpr bool value =
                not has<in_type, ts...>::value and are_unique<ts...>::value;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// replace
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename replace_type, typename with_type, typename... ts>
        class replace_all;

        template <typename replace_type, typename with_type>
        class replace_all<replace_type, with_type>
        {
        public:
            using type = type_list<>;
        };

        template <typename replace_type, typename with_type, typename in_type, typename... ts>
        class replace_all<replace_type, with_type, in_type, ts...>
        {
            using final_type =
                tti::conditional_type<tti::is_same<replace_type, in_type>, with_type, in_type>;

        public:
            using type = typename add_first<final_type,
                typename replace_all<replace_type, with_type, ts...>::in_type>::type;
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
        static constexpr usize count = _type_list_ops::template count<ts...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize max_size = _type_list_ops::template max_size<0, ts...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize min_size = _type_list_ops::template max_size<0, ts...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize max_align = _type_list_ops::template max_align<0, ts...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize min_align = _type_list_ops::template max_align<0, ts...>::value;

        /// ----------------------------------------------------------------------------------------
        /// # to do
        /// - try to remove the explicit 0 index.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
            requires(i < count)
        using at = typename _type_list_ops::template at<i, 0, ts...>::type;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... ts_to_check>
        static constexpr bool has =
            (_type_list_ops::template has<ts_to_check, ts...>::value and ...);

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tto_replace, typename with_type>
        using replace_all =
            typename _type_list_ops::template replace_all<tto_replace, with_type, ts...>::type;

        /// ----------------------------------------------------------------------------------------
        /// # to do
        /// - try to remove the explicit 0 index.
        /// ----------------------------------------------------------------------------------------
        template <typename in_type>
            requires(has<in_type>)
        static constexpr usize index_of =
            _type_list_ops::template index_of<in_type, 0, ts...>::value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr bool are_unique = _type_list_ops::template are_unique<ts...>::value;
    };
}
