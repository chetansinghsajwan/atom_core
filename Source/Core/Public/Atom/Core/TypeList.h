#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    class TypeList;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class TypeListOps
    {
    public:
        ////
        ///-------------------------------------------------------------------------------------------
        //// Count
        ////
        ///-------------------------------------------------------------------------------------------

        template <typename... Ts>
        class Count
        {
        public:
            static constexpr usize Value = sizeof...(Ts);
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// MaxSize
        ////
        ///-------------------------------------------------------------------------------------------

        template <usize max, typename... Ts>
        class MaxSize;

        template <usize max, typename T, typename... Ts>
        class MaxSize<max, T, Ts...>
        {
        private:
            static constexpr usize _ThisSize = sizeof(T);

        public:
            static constexpr usize Value =
                MaxSize<(_ThisSize > max ? _ThisSize : max), Ts...>::Value;
        };

        template <usize max>
        class MaxSize<max>
        {
        private:
            static constexpr usize _ThisSize = 0;

        public:
            static constexpr usize Value = _ThisSize > max ? _ThisSize : max;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// MinSize
        ////
        ///-------------------------------------------------------------------------------------------

        template <usize min, typename... Ts>
        class MinSize;

        template <usize min, typename T, typename... Ts>
        class MinSize<min, T, Ts...>
        {
        private:
            static constexpr usize _ThisSize = alignof(T);

        public:
            static constexpr usize Value =
                MinSize<(_ThisSize < min ? _ThisSize : min), Ts...>::Value;
        };

        template <usize min>
        class MinSize<min>
        {
        private:
            static constexpr usize _ThisSize = 0;

        public:
            static constexpr usize Value = _ThisSize < min ? _ThisSize : min;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// MaxAlign
        ////
        ///-------------------------------------------------------------------------------------------

        template <usize max, typename... Ts>
        class MaxAlign;

        template <usize max, typename T, typename... Ts>
        class MaxAlign<max, T, Ts...>
        {
        private:
            static constexpr usize _ThisAlign = alignof(T);

        public:
            static constexpr usize Value =
                MaxAlign<(_ThisAlign > max ? _ThisAlign : max), Ts...>::Value;
        };

        template <usize max>
        class MaxAlign<max>
        {
        private:
            static constexpr usize _ThisAlign = 0;

        public:
            static constexpr usize Value = _ThisAlign > max ? _ThisAlign : max;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// MinAlign
        ////
        ///-------------------------------------------------------------------------------------------

        template <usize min, typename... Ts>
        class MinAlign;

        template <usize min, typename T, typename... Ts>
        class MinAlign<min, T, Ts...>
        {
        private:
            static constexpr usize _ThisAlign = sizeof(T);

        public:
            static constexpr usize Value =
                MinAlign<(_ThisAlign < min ? _ThisAlign : min), Ts...>::Value;
        };

        template <usize min>
        class MinAlign<min>
        {
        private:
            static constexpr usize _ThisAlign = 0;

        public:
            static constexpr usize Value = _ThisAlign < min ? _ThisAlign : min;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// At
        ////
        ///-------------------------------------------------------------------------------------------

        template <usize indexToGet, usize index, typename... Ts>
        class At;

        template <usize indexToGet, usize index, typename T, typename... Ts>
        class At<indexToGet, index, T, Ts...>
        {
        public:
            using Type = TTI::TConditional<indexToGet == index, T,
                typename At<indexToGet, index + 1, Ts...>::Type>;
        };

        template <usize indexToGet, usize index>
        class At<indexToGet, index>
        {
        public:
            using Type = void;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// IndexOf
        ////
        ///-------------------------------------------------------------------------------------------

        template <typename TToGet, usize index, typename... Ts>
        class IndexOf;

        template <typename TToGet, usize index, typename T, typename... Ts>
        class IndexOf<TToGet, index, T, Ts...>
        {
        public:
            static constexpr usize Value =
                RSameAs<TToGet, T> ? index : IndexOf<TToGet, index + 1, Ts...>::Value;
        };

        template <typename TToGet, usize index>
        class IndexOf<TToGet, index>
        {
        public:
            static constexpr usize Value = -1;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// Has
        ////
        ///-------------------------------------------------------------------------------------------

        template <typename T, typename... Ts>
        class Has
        {
            // TODO: try to remove the explicit 0 index.
        public:
            static constexpr bool Value = IndexOf<T, 0, Ts...>::Value != -1;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// AddFirst
        ////
        ///-------------------------------------------------------------------------------------------

        template <typename T, typename TList>
        class AddFirst;

        template <typename T, typename... Ts>
        class AddFirst<T, TypeList<Ts...>>
        {
        private:
            using Type = TypeList<T, Ts...>;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// AddLast
        ////
        ///-------------------------------------------------------------------------------------------

        template <typename T, typename TList>
        class AddLast;

        template <typename T, typename... Ts>
        class AddLast<T, TypeList<Ts...>>
        {
        public:
            using Type = TypeList<Ts..., T>;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// RemoveIf
        ////
        ///-------------------------------------------------------------------------------------------

        template <template <typename T> typename TPred, typename... Ts>
        class RemoveIf;

        template <template <typename T> typename TPred>
        class RemoveIf<TPred>
        {
            typedef TypeList<> Type;
        };

        template <template <typename T> typename TPred, typename T, typename... Ts>
        class RemoveIf<TPred, T, Ts...>
        {
        public:
            using Type = TTI::TConditional<TPred<T>::Value,
                typename AddFirst<T, typename RemoveIf<TPred, Ts...>::Type>::Type,
                typename RemoveIf<TPred, Ts...>::Type>;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// Remove
        ////
        ///-------------------------------------------------------------------------------------------

        template <typename T, typename... Ts>
        class Remove
        {
            template <typename TCheck>
            class _Pred
            {
                static constexpr bool Value = RSameAs<T, TCheck>;
            };

        public:
            using Type = typename RemoveIf<_Pred, Ts...>::Type;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// RemoveFirst
        ////
        ///-------------------------------------------------------------------------------------------

        template <typename... Ts>
        class RemoveFirst;

        template <typename T, typename... Ts>
        class RemoveFirst<T, Ts...>
        {
        public:
            using Type = TypeList<Ts...>;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// RemoveLast
        ////
        ///-------------------------------------------------------------------------------------------

        template <typename... Ts>
        class RemoveLast;

        template <typename T, typename... Ts>
        class RemoveLast<T, Ts...>
        {
        public:
            using Type = typename AddFirst<T, RemoveLast<Ts...>>::Type;
        };

        template <typename T>
        class RemoveLast<T>
        {
        public:
            using Type = TypeList<>;
        };

        ////
        ///-------------------------------------------------------------------------------------------
        //// AreUnique
        ////
        ///-------------------------------------------------------------------------------------------

        template <typename... Ts>
        class AreUnique;

        template <typename T>
        class AreUnique<T>
        {
        public:
            static constexpr bool Value = true;
        };

        template <typename T, typename... Ts>
        class AreUnique<T, Ts...>
        {
        public:
            static constexpr bool Value = !Has<T, Ts...>::Value && AreUnique<Ts...>::Value;
        };
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    class TypeList
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize Count = TypeListOps::template Count<Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize MaxSize = TypeListOps::template MaxSize<0, Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize MinSize = TypeListOps::template MaxSize<0, Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize MaxAlign = TypeListOps::template MaxAlign<0, Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr usize MinAlign = TypeListOps::template MaxAlign<0, Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        /// # To Do
        /// - Try to remove the explicit 0 index.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
            requires(i < Count)
        using At = typename TypeListOps::template At<i, 0, Ts...>::Type;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... TsToCheck>
        static constexpr bool Has = (TypeListOps::template Has<TsToCheck, Ts...>::Value and ...);

        /// ----------------------------------------------------------------------------------------
        /// # To Do
        /// - Try to remove the explicit 0 index.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
            requires(Has<T>)
        static constexpr usize IndexOf = TypeListOps::template IndexOf<T, 0, Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr bool AreUnique = TypeListOps::template AreUnique<Ts...>::Value;
    };
}
