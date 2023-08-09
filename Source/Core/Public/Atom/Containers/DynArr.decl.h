#pragma once
#include "_DynArrImplHelper.decl.h"
#include "Atom/Memory/DefaultMemAllocator.h"
#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    template <typename T, typename TAlloc>
    class _DynArrImplBase
    {
    public:
        using TElem = T;

    public:
        cexpr ctor _DynArrImplBase() = default;

        cexpr ctor _DynArrImplBase(NullPtr):
            _arr{ nullptr }, _count{ 0 }, _capacity{ 0 }, _alloc{ } { }

    protected:
        cexpr fn _Data() const -> const T*
        {
            return _arr;
        }

        cexpr fn _Data() -> T*
        {
            return _arr;
        }

        cexpr fn _Data(T* arr) -> void
        {
            _arr = arr;
        }

        cexpr fn _Count() const -> usize
        {
            return _count;
        }

        cexpr fn _Count(usize count) -> void
        {
            _count = count;
        }

        cexpr fn _Capacity() const -> usize
        {
            return _capacity;
        }

        cexpr fn _Capacity(usize cap) -> void
        {
            _capacity = cap;
        }

        cexpr fn _AllocMem(usize size) -> T*
        {
            return (T*)_alloc.Alloc(size);
        }

        cexpr fn _DeallocMem(T* mem) -> void
        {
            return _alloc.Dealloc(mem);
        }

        cexpr fn _CalcCapGrowth(usize required) const -> usize
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        cexpr fn _Swap(_DynArrImplBase& that) -> void
        {
            _DynArrImplBase tmp = that;
            self = that;
            that = tmp;
        }

        cexpr fn _Move(_DynArrImplBase& that) -> void
        {
            self = that;
            that = _DynArrImplBase(nullptr);
        }

    protected:
        T* _arr;
        usize _count;
        usize _capacity;
        TAlloc _alloc;
    };

    template <typename T, typename TAlloc = DefaultMemAllocator>
    class DynArr extends _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>
    {
        priv using Base = _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>;
        priv using BaseImpl = _DynArrImplBase<T, TAlloc>;

        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor DynArr() = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor DynArr(NullPtr):
            Base{BaseImpl{ nullptr }} { }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        pub template <typename TRange>
        requires RRangeOf<TRange, T>
        cexpr ctor DynArr(const TRange& range):
            Base{BaseImpl{ nullptr }}
        {
            self.InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor DynArr(const DynArr& that):
            Base{BaseImpl{ nullptr }}
        {
            self.InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op=(const DynArr& that) -> DynArr&
        {
            self.Clear();
            self.InsertBack(that);
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor DynArr(DynArr&& that):
            Base{BaseImpl{ mov(that) }} { }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op=(DynArr&& that) -> DynArr&
        {
            DynArr tmp = mov(that);
            self._Swap(tmp);
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr dtor DynArr()
        {
            self.Clear();
            self.Release();
        }
    };
}