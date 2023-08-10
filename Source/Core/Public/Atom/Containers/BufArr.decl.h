#pragma once
#include "DynArr.decl.h"

namespace Atom
{
    template <typename T, usize BufSize, typename TAlloc>
    class _BufArrImplBase extends _DynArrImplBase<T, TAlloc>
    {
        priv using Base = _DynArrImplBase<T, TAlloc>;
        prot using TElem = T;
        prot using Base::Base;

        prot constexpr fn _StackBuf() const -> const T*
        {
            return _stackBuf;
        }

        prot constexpr fn _AllocMem(usize size) -> T*
        {
            if (BufSize <= size)
            {
                return _stackBuf;
            }

            return Base::_AllocMem(size);
        }

        prot constexpr fn _DeallocMem(T* mem) -> void
        {
            if (mem == _stackBuf)
            {
                return;
            }

            return Base::_DeallocMem(mem);
        }

        prot constexpr fn _CalcCapGrowth(usize required) const -> usize
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        prot using Base::_Count;
        prot using Base::_Capacity;

        prot T _stackBuf[BufSize];
    };

    template <typename T, usize bufSize, typename TAlloc>
    class BufArr extends _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>
    {
        priv using Base = _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>;
        priv using BaseImpl = _BufArrImplBase<T, bufSize, TAlloc>;

        pub using TElem = typename Base::TElem;

        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        pub constexpr ctor BufArr() = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        pub constexpr ctor BufArr(NullPtr):
            Base{ nullptr } { }

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        pub constexpr fn operator=(NullPtr) -> BufArr&
        {
            Clear();
            Release();
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor for Range.
        /// ----------------------------------------------------------------------------------------
        pub template <typename TRange>
        requires RRangeOf<TRange, T>
        constexpr ctor BufArr(TRange&& range):
            Base{ nullptr }
        {
            InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper for Range.
        /// ----------------------------------------------------------------------------------------
        pub template <typename TRange>
        requires RRangeOf<TRange, T>
        constexpr fn operator=(TRange&& range) -> BufArr&
        {
            Clear();
            InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyCtor.
        /// 
        /// @TODO: Check if we need this ctor to satisfy std::is_copy_constructible and 
        ///     RCopyConstructible.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor BufArr(const BufArr& that):
            Base{ nullptr }
        {
            // InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// 
        /// @TODO: Same as CopyCtor.
        /// ----------------------------------------------------------------------------------------
        pub constexpr fn operator=(const BufArr& that) -> BufArr&
        {
            Clear();
            InsertBack(that);
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        pub constexpr ctor BufArr(BufArr&& that):
            Base{ nullptr }
        {
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveCtor.
        /// ----------------------------------------------------------------------------------------
        pub template <usize thatBufSize>
        constexpr ctor BufArr(BufArr<TElem, thatBufSize, TAlloc>&& that):
            Base{ nullptr }
        {
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor for DynArr.
        /// ----------------------------------------------------------------------------------------
        pub constexpr ctor BufArr(DynArr<TElem, TAlloc>&& that):
            Base{ nullptr }
        {
            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        pub constexpr fn operator=(BufArr&& that) -> BufArr&
        {
            Clear();
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveOper.
        /// ----------------------------------------------------------------------------------------
        pub template <usize thatBufSize>
        constexpr fn operator=(BufArr<TElem, thatBufSize, TAlloc>&& that) -> BufArr&
        {
            Clear();
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper for DynArr.
        /// ----------------------------------------------------------------------------------------
        pub constexpr fn operator=(DynArr<TElem, TAlloc>&& that) -> BufArr&
        {
            Clear();
            Release();

            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        pub constexpr dtor BufArr()
        {
            Clear();
            Release();
        }

        pub using Base::Clear;
        pub using Base::Release;
        pub using Base::InsertBack;

        /// ----------------------------------------------------------------------------------------
        /// @EXPECTS Empty().
        /// ----------------------------------------------------------------------------------------
        prot template <usize thatBufSize>
        fn _Move(BufArr<TElem, thatBufSize, TAlloc>&& that)
        {
            if (that._Data() == that._StackBuf())
            {
                _EnsureCapFor(that._Count());
                that._MoveRangeTo(0, _Data());
                return;
            }

            _DynArrImplBase<TElem, TAlloc>::_Move(that);
        }

        using Base::_EnsureCapFor;
        using Base::_Data;
    };
}