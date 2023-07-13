#pragma once
#include "_DynArrImplHelper.decl.h"
#include "Atom/Memory/DefaultMemAllocator.h"
#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    template <tname T, tname TAlloc>
    class _DynArrImplBase
    {
    public:
        using TElem = T;

    public:
        cexpr ctor _DynArrImplBase() noex = default;

        cexpr ctor _DynArrImplBase(NullPtr) noex:
            _arr{ nullptr }, _count{ 0 }, _capacity{ 0 }, _alloc{ } { }

    protected:
        cexpr fn _Data() const noex -> const T*
        {
            return _arr;
        }

        cexpr fn _Data() noex -> T*
        {
            return _arr;
        }

        cexpr fn _Data(T* arr) noex -> void
        {
            _arr = arr;
        }

        cexpr fn _Count() const noex -> usize
        {
            return _count;
        }

        cexpr fn _Count(usize count) noex -> void
        {
            _count = count;
        }

        cexpr fn _Capacity() const noex -> usize
        {
            return _capacity;
        }

        cexpr fn _Capacity(usize cap) noex -> void
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

        cexpr fn _CalcCapGrowth(usize required) const noex -> usize
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        cexpr fn _Swap(_DynArrImplBase& that) noex -> void
        {
            _DynArrImplBase tmp = that;
            *this = that;
            that = tmp;
        }

        cexpr fn _Move(_DynArrImplBase& that) noex -> void
        {
            *this = that;
            that = _DynArrImplBase(nullptr);
        }

    protected:
        T* _arr;
        usize _count;
        usize _capacity;
        TAlloc _alloc;
    };

    template <tname T, tname TAlloc = DefaultMemAllocator>
    class DynArr extends _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>
    {
        pri using Base = _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>;
        pri using BaseImpl = _DynArrImplBase<T, TAlloc>;

        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor DynArr() noex = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor DynArr(NullPtr) noex:
            Base{BaseImpl{ nullptr }} { }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        pub template <tname TRange>
        requires RRangeOf<TRange, T>
        cexpr ctor DynArr(const TRange& range) noex:
            Base{BaseImpl{ nullptr }}
        {
            this->InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor DynArr(const DynArr& that) noex:
            Base{BaseImpl{ nullptr }}
        {
            this->InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn operator =(const DynArr& that) noex -> DynArr&
        {
            this->Clear();
            this->InsertBack(that);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor DynArr(DynArr&& that) noex:
            Base{BaseImpl{ MOVE(that) }} { }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn operator =(DynArr&& that) noex -> DynArr&
        {
            DynArr tmp = MOVE(that);
            this->_Swap(tmp);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr dtor DynArr() noex
        {
            this->Clear();
            this->Release();
        }
    };
}