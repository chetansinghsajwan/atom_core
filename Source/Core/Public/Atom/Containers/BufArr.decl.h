#pragma once
#include "DynArr.decl.h"

namespace Atom
{
    template <typename T, usize BufSize, typename TAlloc>
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

        constexpr auto _CalcCapGrowth(usize required) const noexcept -> usize
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
    class BufArr: public _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>
    {
        using Base = _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>;
        using BaseImpl = _BufArrImplBase<T, bufSize, TAlloc>;

    public:
        using TElem = typename Base::TElem;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr() noexcept = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr(NullPtr) noexcept:
            Base{ nullptr } { }

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr& operator =(NullPtr) noexcept
        {
            Clear();
            Release();
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor for Range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRangeOf<TRange, T>
        constexpr BufArr(TRange&& range) noexcept:
            Base{ nullptr }
        {
            InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper for Range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRangeOf<TRange, T>
        constexpr BufArr& operator =(TRange&& range) noexcept
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
        constexpr BufArr(const BufArr& that) noexcept:
            Base{ nullptr }
        {
            // InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// 
        /// @TODO: Same as CopyCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr& operator =(const BufArr& that) noexcept
        {
            Clear();
            InsertBack(that);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr(BufArr&& that) noexcept:
            Base{ nullptr }
        {
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveCtor.
        /// ----------------------------------------------------------------------------------------
        template <usize thatBufSize>
        constexpr BufArr(BufArr<TElem, thatBufSize, TAlloc>&& that) noexcept:
            Base{ nullptr }
        {
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor for DynArr.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr(DynArr<TElem, TAlloc>&& that) noexcept:
            Base{ nullptr }
        {
            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr& operator =(BufArr&& that) noexcept
        {
            Clear();
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveOper.
        /// ----------------------------------------------------------------------------------------
        template <usize thatBufSize>
        constexpr BufArr& operator =(BufArr<TElem, thatBufSize, TAlloc>&& that) noexcept
        {
            Clear();
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper for DynArr.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr& operator =(DynArr<TElem, TAlloc>&& that) noexcept
        {
            Clear();
            Release();

            BaseImpl::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ~BufArr() noexcept
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
        void _Move(BufArr<TElem, thatBufSize, TAlloc>&& that)
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