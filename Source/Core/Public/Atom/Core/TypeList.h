#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    class TypeList;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    class TypeListOps
    {
    public:
    //// -------------------------------------------------------------------------------------------
    //// Count
    //// -------------------------------------------------------------------------------------------

        template <tname... Ts>
        class Count
        {
            pub static cexpr usize Value = sizeof...(Ts);
        };

    //// -------------------------------------------------------------------------------------------
    //// MaxSize
    //// -------------------------------------------------------------------------------------------

        template <usize max, tname... Ts>
        class MaxSize;

        template <usize max, tname T, tname... Ts>
        class MaxSize<max, T, Ts...>
        {
            priv static cexpr usize _ThisSize = sizeof(T);
            pub static cexpr usize Value = MaxSize<(_ThisSize > max ? _ThisSize : max),
                Ts...>::Value;
        };

        template <usize max>
        class MaxSize<max>
        {
            priv static cexpr usize _ThisSize = 0;
            pub static cexpr usize Value = _ThisSize > max ? _ThisSize : max;
        };

    //// -------------------------------------------------------------------------------------------
    //// MinSize
    //// -------------------------------------------------------------------------------------------

        template <usize min, tname... Ts>
        class MinSize;

        template <usize min, tname T, tname... Ts>
        class MinSize<min, T, Ts...>
        {
            priv static cexpr usize _ThisSize = alignof(T);
            pub static cexpr usize Value = MinSize<(_ThisSize < min ? _ThisSize : min),
                Ts...>::Value;
        };

        template <usize min>
        class MinSize<min>
        {
            priv static cexpr usize _ThisSize = 0;
            pub static cexpr usize Value = _ThisSize < min ? _ThisSize : min;
        };

    //// -------------------------------------------------------------------------------------------
    //// MaxAlign
    //// -------------------------------------------------------------------------------------------

        template <usize max, tname... Ts>
        class MaxAlign;

        template <usize max, tname T, tname... Ts>
        class MaxAlign<max, T, Ts...>
        {
            priv static cexpr usize _ThisAlign = alignof(T);
            pub static cexpr usize Value = MaxAlign<(_ThisAlign > max ? _ThisAlign : max),
                Ts...>::Value;
        };

        template <usize max>
        class MaxAlign<max>
        {
            priv static cexpr usize _ThisAlign = 0;
            pub static cexpr usize Value = _ThisAlign > max ? _ThisAlign : max;
        };

    //// -------------------------------------------------------------------------------------------
    //// MinAlign
    //// -------------------------------------------------------------------------------------------

        template <usize min, tname... Ts>
        class MinAlign;

        template <usize min, tname T, tname... Ts>
        class MinAlign<min, T, Ts...>
        {
            priv static cexpr usize _ThisAlign = sizeof(T);
            pub static cexpr usize Value = MinAlign<(_ThisAlign < min ? _ThisAlign : min),
                Ts...>::Value;
        };

        template <usize min>
        class MinAlign<min>
        {
            priv static cexpr usize _ThisAlign = 0;
            pub static cexpr usize Value = _ThisAlign < min ? _ThisAlign : min;
        };

    //// -------------------------------------------------------------------------------------------
    //// At
    //// -------------------------------------------------------------------------------------------

        template <usize indexToGet, usize index, tname... Ts>
        class At;

        template <usize indexToGet, usize index, tname T, tname... Ts>
        class At<indexToGet, index, T, Ts...>
        {
            pub using Type = TTI::TConditional<indexToGet == index, T,
                tname At<indexToGet, index + 1, Ts...>::Type>;
        };

        template <usize indexToGet, usize index>
        class At<indexToGet, index>
        {
            pub using Type = void;
        };

    //// -------------------------------------------------------------------------------------------
    //// IndexOf
    //// -------------------------------------------------------------------------------------------

        template <tname TToGet, usize index, tname... Ts>
        class IndexOf;

        template <tname TToGet, usize index, tname T, tname... Ts>
        class IndexOf<TToGet, index, T, Ts...>
        {
            pub static cexpr usize Value = RSameAs<TToGet, T> ? index :
                IndexOf<TToGet, index + 1, Ts...>::Value;
        };

        template <tname TToGet, usize index>
        class IndexOf<TToGet, index>
        {
            pub static cexpr usize Value = -1;
        };

    //// -------------------------------------------------------------------------------------------
    //// Has
    //// -------------------------------------------------------------------------------------------

        template <tname T, tname... Ts>
        class Has
        {
            // TODO: try to remove the explicit 0 index.
            pub static cexpr bool Value = IndexOf<T, 0, Ts...>::Value != -1;
        };

    //// -------------------------------------------------------------------------------------------
    //// AddFirst
    //// -------------------------------------------------------------------------------------------

        template <tname T, tname TList>
        class AddFirst;

        template <tname T, tname... Ts>
        class AddFirst<T, TypeList<Ts...>>
        {
            using Type = TypeList<T, Ts...>;
        };

    //// -------------------------------------------------------------------------------------------
    //// AddLast
    //// -------------------------------------------------------------------------------------------

        template <tname T, tname TList>
        class AddLast;

        template <tname T, tname... Ts>
        class AddLast<T, TypeList<Ts...>>
        {
            pub using Type = TypeList<Ts..., T>;
        };

    //// -------------------------------------------------------------------------------------------
    //// RemoveIf
    //// -------------------------------------------------------------------------------------------

        template <template <tname T> tname TPred, tname... Ts>
        class RemoveIf;

        template <template <tname T> tname TPred>
        class RemoveIf<TPred>
        {
            typedef TypeList<> Type;
        };

        template <template <tname T> tname TPred, tname T, tname... Ts>
        class RemoveIf<TPred, T, Ts...>
        {
            pub using Type = TTI::TConditional
            <
                TPred<T>::Value,
                tname AddFirst<T, tname RemoveIf<TPred, Ts...>::Type>::Type,
                tname RemoveIf<TPred, Ts...>::Type
            >;
        };

    //// -------------------------------------------------------------------------------------------
    //// Remove
    //// -------------------------------------------------------------------------------------------

        template <tname T, tname... Ts>
        class Remove
        {
            template <tname TCheck>
            class _Pred
            {
                static constexpr bool Value = RSameAs<T, TCheck>;
            };

            pub using Type = tname RemoveIf<_Pred, Ts...>::Type;
        };

    //// -------------------------------------------------------------------------------------------
    //// RemoveFirst
    //// -------------------------------------------------------------------------------------------

        template <tname... Ts>
        class RemoveFirst;

        template <tname T, tname... Ts>
        class RemoveFirst<T, Ts...>
        {
            pub using Type = TypeList<Ts...>;
        };

    //// -------------------------------------------------------------------------------------------
    //// RemoveLast
    //// -------------------------------------------------------------------------------------------

        template <tname... Ts>
        class RemoveLast;

        template <tname T, tname... Ts>
        class RemoveLast<T, Ts...>
        {
            pub using Type = tname AddFirst<T, RemoveLast<Ts...>>::Type;
        };

        template <tname T>
        class RemoveLast<T>
        {
            pub using Type = TypeList<>;
        };

    //// -------------------------------------------------------------------------------------------
    //// AreUnique
    //// -------------------------------------------------------------------------------------------

        template <tname... Ts>
        class AreUnique;

        template <tname T>
        class AreUnique <T>
        {
            pub static cexpr bool Value = true;
        };

        template <tname T, tname... Ts>
        class AreUnique <T, Ts...>
        {
            pub static cexpr bool Value = !Has<T, Ts...>::Value && AreUnique<Ts...>::Value;
        };
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    class TypeList
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static cexpr usize Count = TypeListOps::template Count<Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static cexpr usize MaxSize = TypeListOps::template MaxSize<0, Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static cexpr usize MinSize = TypeListOps::template MaxSize<0, Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static cexpr usize MaxAlign = TypeListOps::template MaxAlign<0, Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static cexpr usize MinAlign = TypeListOps::template MaxAlign<0, Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        /// # To Do
        /// - Try to remove the explicit 0 index.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        requires (i < Count)
        using At = tname TypeListOps::template At<i, 0, Ts...>::Type;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        static cexpr bool Has = TypeListOps::template Has<T, Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        /// # To Do
        /// - Try to remove the explicit 0 index.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        requires (Has<T>)
        static cexpr usize IndexOf = TypeListOps::template IndexOf<T, 0, Ts...>::Value;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static cexpr bool AreUnique = TypeListOps::template AreUnique<Ts...>::Value;
    };
}