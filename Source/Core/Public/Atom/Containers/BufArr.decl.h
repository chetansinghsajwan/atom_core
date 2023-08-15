#pragma once
#include "DynArr.decl.h"

namespace Atom
{
    template <typename TAlloc>
    class _BufArrAllocWrap: public TAlloc {};
    
    template <typename T, usize bufSize, typename TAlloc>
    class _DynArrStorage<T, _BufArrAllocWrap<TAlloc>>: public _DynArrStorage<T, TAlloc>
    {
    private:
        using _Impl = _DynArrStorage<T, TAlloc>;

    protected:
        constexpr auto _StackBuf() const -> const T*
        {
            return _stackBuf;
        }

        constexpr auto _AllocMem(usize size) -> T*
        {
            if (bufSize <= size)
            {
                return _stackBuf;
            }

            return _Impl::_AllocMem(size);
        }

        constexpr auto _DeallocMem(T* mem) -> void
        {
            if (mem == _stackBuf)
            {
                return;
            }

            return _Impl::_DeallocMem(mem);
        }

        constexpr auto _CalcCapGrowth(usize required) const -> usize
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        using _Impl::_Capacity;
        using _Impl::_Count;

    protected:
        T _stackBuf[bufSize];
    };

    template <typename T, usize bufSize, typename TAlloc>
    class BufArr: public DynArr<T, _BufArrAllocWrap<TAlloc>>
    {
    private:
        using _Impl = _DynArrImpl<_BufArrStorage<T, bufSize, TAlloc>>;
        using _Storage = _BufArrStorage<T, bufSize, TAlloc>;

    public:
        using TElem = typename _Impl::TElem;

        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr() = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr(NullPtr)
            : _Impl{ nullptr }
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
            : _Impl{ nullptr }
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
            : _Impl{ nullptr }
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
            : _Impl{ nullptr }
        {
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveCtor.
        /// ----------------------------------------------------------------------------------------
        template <usize thatBufSize>
        constexpr BufArr(BufArr<TElem, thatBufSize, TAlloc>&& that)
            : _Impl{ nullptr }
        {
            _Move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor for DynArr.
        /// ----------------------------------------------------------------------------------------
        constexpr BufArr(DynArr<TElem, TAlloc>&& that)
            : _Impl{ nullptr }
        {
            _Storage::_Move(that);
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

            _Storage::_Move(that);
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
        using _Impl::Clear;
        using _Impl::InsertBack;
        using _Impl::Release;

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

            _DynArrStorage<TElem, TAlloc>::_Move(that);
        }

        using _Impl::_Data;
        using _Impl::_EnsureCapFor;
    };
}
