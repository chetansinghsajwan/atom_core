#pragma once
#include "DynArr.decl.h"

namespace Atom
{
    template <tname T, usize BufSize, tname TAlloc>
    class _BufArrImplBase extends _DynArrImplBase<T, TAlloc>
    {
        priv using Base = _DynArrImplBase<T, TAlloc>;
        prot using TElem = T;
        prot using Base::Base;

        prot cexpr fn _StackBuf() const -> const T*
        {
            return _stackBuf;
        }

        prot cexpr fn _AllocMem(usize size) -> T*
        {
            if (BufSize <= size)
            {
                return _stackBuf;
            }

            return Base::_AllocMem(size);
        }

        prot cexpr fn _DeallocMem(T* mem) -> void
        {
            if (mem == _stackBuf)
            {
                return;
            }

            return Base::_DeallocMem(mem);
        }

        prot cexpr fn _CalcCapGrowth(usize required) const noex -> usize
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        prot using Base::_Count;
        prot using Base::_Capacity;

        prot T _stackBuf[BufSize];
    };

    template <tname T, usize bufSize, tname TAlloc>
    class BufArr extends _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>
    {
        priv using Base = _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>;
        priv using BaseImpl = _BufArrImplBase<T, bufSize, TAlloc>;

        pub using TElem = tname Base::TElem;

        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor BufArr() noex = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor BufArr(NullPtr) noex:
            Base{ nullptr } { }

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op=(NullPtr) noex -> BufArr&
        {
            Clear();
            Release();
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor for Range.
        /// ----------------------------------------------------------------------------------------
        pub template <tname TRange>
        requires RRangeOf<TRange, T>
        cexpr ctor BufArr(TRange&& range) noex:
            Base{ nullptr }
        {
            InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper for Range.
        /// ----------------------------------------------------------------------------------------
        pub template <tname TRange>
        requires RRangeOf<TRange, T>
        cexpr fn op=(TRange&& range) noex -> BufArr&
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
        cexpr ctor BufArr(const BufArr& that) noex:
            Base{ nullptr }
        {
            // InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// 
        /// @TODO: Same as CopyCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op=(const BufArr& that) noex -> BufArr&
        {
            Clear();
            InsertBack(that);
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor BufArr(BufArr&& that) noex:
            Base{ nullptr }
        {
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveCtor.
        /// ----------------------------------------------------------------------------------------
        pub template <usize thatBufSize>
        cexpr ctor BufArr(BufArr<TElem, thatBufSize, TAlloc>&& that) noex:
            Base{ nullptr }
        {
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor for DynArr.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor BufArr(DynArr<TElem, TAlloc>&& that) noex:
            Base{ nullptr }
        {
            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op=(BufArr&& that) noex -> BufArr&
        {
            Clear();
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveOper.
        /// ----------------------------------------------------------------------------------------
        pub template <usize thatBufSize>
        cexpr fn op=(BufArr<TElem, thatBufSize, TAlloc>&& that) noex -> BufArr&
        {
            Clear();
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper for DynArr.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op=(DynArr<TElem, TAlloc>&& that) noex -> BufArr&
        {
            Clear();
            Release();

            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr dtor BufArr() noex
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