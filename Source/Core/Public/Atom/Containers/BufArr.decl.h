#pragma once
#include "DynArr.decl.h"

namespace Atom
{
    template <typename T, usize BufSize, typename TAlloc>
    class _BufArrImplBase: public _DynArrImplBase<T, TAlloc>
    {
    private:
        using Base = _DynArrImplBase<T, TAlloc>;

    protected:
        using TElem = T;
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

        constexpr auto _CalcCapGrowth(usize required) const -> usize
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        using Base::_Capacity;
        using Base::_Count;

    protected:
        T _stackBuf[BufSize];
    };

    template <typename T, usize bufSize, typename TAlloc>
    class BufArr: public _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>
    {
    private:
        using Base = _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>;
        using BaseImpl = _BufArrImplBase<T, bufSize, TAlloc>;

    public:
        using TElem = typename Base::TElem;

        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr() = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr(NullPtr)
            : Base{ nullptr }
        {}

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(NullPtr) -> BufArr&
        {
            Clear();
            Release();
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor for Range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr BufArr(TRange&& range)
            requires(RRangeOf<TRange, T>)
            : Base{ nullptr }
        {
            InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper for Range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto operator=(TRange&& range) -> BufArr& requires(RRangeOf<TRange, T>) {
            Clear();
            InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyCtor.
        ///
        /// @TODO: Check if we need this to satisfy std::is_copy_constructible and
        ///     RCopyConstructible.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr(const BufArr& that)
            : Base{ nullptr }
        {
            // InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        ///
        /// @TODO: Same as CopyCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const BufArr& that) -> BufArr&
        {
            Clear();
            InsertBack(that);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr(BufArr&& that)
            : Base{ nullptr }
        {
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveCtor.
        /// ----------------------------------------------------------------------------------------
        template <usize thatBufSize>
        constexpr BufArr(BufArr<TElem, thatBufSize, TAlloc>&& that)
            : Base{ nullptr }
        {
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor for DynArr.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr(DynArr<TElem, TAlloc>&& that)
            : Base{ nullptr }
        {
            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(BufArr&& that) -> BufArr&
        {
            Clear();
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveOper.
        /// ----------------------------------------------------------------------------------------
        template <usize thatBufSize>
        constexpr auto operator=(BufArr<TElem, thatBufSize, TAlloc>&& that) -> BufArr&
        {
            Clear();
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper for DynArr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(DynArr<TElem, TAlloc>&& that) -> BufArr&
        {
            Clear();
            Release();

            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ~BufArr()
        {
            Clear();
            Release();
        }

    public:
        using Base::Clear;
        using Base::InsertBack;
        using Base::Release;

    protected:
        /// ----------------------------------------------------------------------------------------
        /// @EXPECTS Empty().
        /// ----------------------------------------------------------------------------------------
        template <usize thatBufSize>
        auto _Move(BufArr<TElem, thatBufSize, TAlloc>&& that)
        {
            if (that._Data() == that._StackBuf())
            {
                _EnsureCapFor(that._Count());
                that._MoveRangeTo(0, _Data());
                return;
            }

            _DynArrImplBase<TElem, TAlloc>::_Move(that);
        }

        using Base::_Data;
        using Base::_EnsureCapFor;
    };
}
