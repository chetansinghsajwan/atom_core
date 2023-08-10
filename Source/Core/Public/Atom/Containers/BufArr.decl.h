#pragma once
#include "DynArr.decl.h"

namespace Atom
{
    template <typename T, usize BufSize, typename TAlloc>
    class _BufArrImplBase extends _DynArrImplBase<T, TAlloc>
    {
    private:
        using Base = _DynArrImplBase<T, TAlloc>;

    protected:
        using TElem = T;
        using Base::Base;

    protected:
        constexpr fn _StackBuf() const -> const T*
        {
            return _stackBuf;
        }

        constexpr fn _AllocMem(usize size) -> T*
        {
            if (BufSize <= size)
            {
                return _stackBuf;
            }

            return Base::_AllocMem(size);
        }

        constexpr fn _DeallocMem(T* mem) -> void
        {
            if (mem == _stackBuf)
            {
                return;
            }

            return Base::_DeallocMem(mem);
        }

        constexpr fn _CalcCapGrowth(usize required) const -> usize
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        using Base::_Count;
        using Base::_Capacity;

    protected:
        T _stackBuf[BufSize];
    };

    template <typename T, usize bufSize, typename TAlloc>
    class BufArr extends _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>
    {
    private:
        using Base = _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>;
        using BaseImpl = _BufArrImplBase<T, bufSize, TAlloc>;

    public:
        using TElem = typename Base::TElem;

        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor BufArr() = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor BufArr(NullPtr):
            Base{ nullptr } { }

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        constexpr fn operator=(NullPtr) -> BufArr&
        {
            Clear();
            Release();
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor for Range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRangeOf<TRange, T>
        constexpr ctor BufArr(TRange&& range):
            Base{ nullptr }
        {
            InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper for Range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
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
        constexpr fn operator=(const BufArr& that) -> BufArr&
        {
            Clear();
            InsertBack(that);
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor BufArr(BufArr&& that):
            Base{ nullptr }
        {
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveCtor.
        /// ----------------------------------------------------------------------------------------
        template <usize thatBufSize>
        constexpr ctor BufArr(BufArr<TElem, thatBufSize, TAlloc>&& that):
            Base{ nullptr }
        {
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor for DynArr.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor BufArr(DynArr<TElem, TAlloc>&& that):
            Base{ nullptr }
        {
            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        constexpr fn operator=(BufArr&& that) -> BufArr&
        {
            Clear();
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveOper.
        /// ----------------------------------------------------------------------------------------
        template <usize thatBufSize>
        constexpr fn operator=(BufArr<TElem, thatBufSize, TAlloc>&& that) -> BufArr&
        {
            Clear();
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper for DynArr.
        /// ----------------------------------------------------------------------------------------
        constexpr fn operator=(DynArr<TElem, TAlloc>&& that) -> BufArr&
        {
            Clear();
            Release();

            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        constexpr dtor BufArr()
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