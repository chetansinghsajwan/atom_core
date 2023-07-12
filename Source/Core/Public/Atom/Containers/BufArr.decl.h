#pragma once
#include "DynArr.decl.h"

namespace Atom
{
    template <tname T, usize BufSize, tname TAlloc>
    class _BufArrImplBase : protected _DynArrImplBase<T, TAlloc>
    {
    private:
        using Base = _DynArrImplBase<T, TAlloc>;

    public:
        using TElem = T;

    public:
        using Base::Base;

    protected:
        constexpr auto _StackBuf() const -> const T*
        {
            return _stackBuf;
        }

        constexpr auto _AllocMem(usize size) -> T*
        {
            if (BufSize <= size)
            {
                return _stackBuf;
            }

            return Base::_AllocMem(size);
        }

        constexpr auto _DeallocMem(T* mem) -> void
        {
            if (mem == _stackBuf)
            {
                return;
            }

            return Base::_DeallocMem(mem);
        }

        constexpr auto _CalcCapGrowth(usize required) const noex -> usize
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        using Base::_Count;
        using Base::_Capacity;

    protected:
        T _stackBuf[BufSize];
    };

    template <tname T, usize bufSize, tname TAlloc>
    class BufArr: public _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>
    {
        using Base = _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>;
        using BaseImpl = _BufArrImplBase<T, bufSize, TAlloc>;

    public:
        using TElem = tname Base::TElem;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor BufArr() noex = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor BufArr(NullPtr) noex:
            Base{ nullptr } { }

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        constexpr fn operator =(NullPtr) noex -> BufArr&
        {
            Clear();
            Release();
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor for Range.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RRangeOf<TRange, T>
        constexpr ctor BufArr(TRange&& range) noex:
            Base{ nullptr }
        {
            InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper for Range.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RRangeOf<TRange, T>
        constexpr fn operator =(TRange&& range) noex -> BufArr&
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
        constexpr ctor BufArr(const BufArr& that) noex:
            Base{ nullptr }
        {
            // InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// 
        /// @TODO: Same as CopyCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr fn operator =(const BufArr& that) noex -> BufArr&
        {
            Clear();
            InsertBack(that);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor BufArr(BufArr&& that) noex:
            Base{ nullptr }
        {
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveCtor.
        /// ----------------------------------------------------------------------------------------
        template <usize thatBufSize>
        constexpr ctor BufArr(BufArr<TElem, thatBufSize, TAlloc>&& that) noex:
            Base{ nullptr }
        {
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor for DynArr.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor BufArr(DynArr<TElem, TAlloc>&& that) noex:
            Base{ nullptr }
        {
            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        constexpr fn operator =(BufArr&& that) noex -> BufArr&
        {
            Clear();
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveOper.
        /// ----------------------------------------------------------------------------------------
        template <usize thatBufSize>
        constexpr fn operator =(BufArr<TElem, thatBufSize, TAlloc>&& that) noex -> BufArr&
        {
            Clear();
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper for DynArr.
        /// ----------------------------------------------------------------------------------------
        constexpr fn operator =(DynArr<TElem, TAlloc>&& that) noex -> BufArr&
        {
            Clear();
            Release();

            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        constexpr dtor BufArr() noex
        {
            Clear();
            Release();
        }

    public:
        using Base::Clear;
        using Base::Release;
        using Base::InsertBack;

    protected:
        /// ----------------------------------------------------------------------------------------
        /// @EXPECTS Empty().
        /// ----------------------------------------------------------------------------------------
        template <usize thatBufSize>
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