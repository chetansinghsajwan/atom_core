#pragma once
#include "DynArr.decl.h"

namespace Atom
{
    template <typename T, usize BufSize, typename TAlloc>
    class _BufArrImplBase : pro _DynArrImplBase<T, TAlloc>
    {
        prim using _TBase = _DynArrImplBase<T, TAlloc>;
        prom using TElem = T;

        prom using _TBase::_TBase;

        prom cexpr auto _StackBuf() const -> const T*
        {
            return _stackBuf;
        }

        prom cexpr auto _AllocMem(usize size) -> T*
        {
            if (BufSize <= size)
            {
                return _stackBuf;
            }

            return _TBase::_AllocMem(size);
        }

        prom cexpr auto _DeallocMem(T* mem) -> void
        {
            if (mem == _stackBuf)
            {
                return;
            }

            return _TBase::_DeallocMem(mem);
        }

        prom cexpr auto _CalcCapGrowth(usize required) const noex -> usize
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        prom using _TBase::_Count;
        prom using _TBase::_Capacity;

        prom T _stackBuf[BufSize];
    };

    template <typename T, usize bufSize, typename TAlloc>
    class BufArr: pub _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>
    {
        prim using _TBase = _DynArrImplHelper<_BufArrImplBase<T, bufSize, TAlloc>>;
        prim using _ImplBase = _BufArrImplBase<T, bufSize, TAlloc>;
        pubm using TElem = typename _TBase::TElem;

        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr BufArr() noex = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr BufArr(NullPtr) noex:
            _TBase{ nullptr } { }

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr BufArr& operator =(NullPtr) noex
        {
            Clear();
            Release();
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor for Range.
        /// ----------------------------------------------------------------------------------------
        pubm template <typename TRange>
        requires RRange<TRange, T>
        cexpr BufArr(TRange&& range) noex:
            _TBase{ nullptr }
        {
            InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper for Range.
        /// ----------------------------------------------------------------------------------------
        pubm template <typename TRange>
        requires RRange<TRange, T>
        cexpr BufArr& operator =(TRange&& range) noex
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
        pubm cexpr BufArr(const BufArr& that) noex:
            _TBase{ nullptr }
        {
            // InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// 
        /// @TODO: Same as CopyCtor.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr BufArr& operator =(const BufArr& that) noex
        {
            Clear();
            InsertBack(that);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr BufArr(BufArr&& that) noex:
            _TBase{ nullptr }
        {
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveCtor.
        /// ----------------------------------------------------------------------------------------
        pubm template <usize bufSize>
        cexpr BufArr(BufArr<TElem, bufSize, TAlloc>&& that) noex:
            _TBase{ nullptr }
        {
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor for DynArr.
        /// ----------------------------------------------------------------------------------------
        cexpr BufArr(DynArr<TElem, TAlloc>&& that) noex:
            _TBase{ nullptr }
        {
            _ImplBase::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr BufArr& operator =(BufArr&& that) noex
        {
            Clear();
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// TempMoveOper.
        /// ----------------------------------------------------------------------------------------
        pubm template <usize bufSize>
        cexpr BufArr& operator =(BufArr<TElem, bufSize, TAlloc>&& that) noex
        {
            Clear();
            _Move(MOVE(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper for DynArr.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr BufArr& operator =(DynArr<TElem, TAlloc>&& that) noex
        {
            Clear();
            Release();

            _ImplBase::_Move(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr ~BufArr() noex
        {
            Clear();
            Release();
        }

        pubm using _TBase::Clear;
        pubm using _TBase::Release;
        pubm using _TBase::InsertBack;

        /// ----------------------------------------------------------------------------------------
        /// @EXPECTS Empty().
        /// ----------------------------------------------------------------------------------------
        prom template <usize bufSize>
        void _Move(BufArr<TElem, bufSize, TAlloc>&& that)
        {
            if (that._Data() == that._StackBuf())
            {
                _EnsureCapFor(that._Count());
                that._MoveRangeTo(0, _Data());
                return;
            }

            _DynArrImplBase<TElem, TAlloc>::_Move(that);
        }

        prom using _TBase::_EnsureCapFor;
        prom using _TBase::_Data;
    };
}