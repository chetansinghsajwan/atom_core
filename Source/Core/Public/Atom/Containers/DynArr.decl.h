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
        constexpr ctor _DynArrImplBase() noex = default;

        constexpr ctor _DynArrImplBase(NullPtr) noex:
            _arr{ nullptr }, _count{ 0 }, _capacity{ 0 }, _alloc{ } { }

    protected:
        constexpr fn _Data() const noex -> const T*
        {
            return _arr;
        }

        constexpr fn _Data() noex -> T*
        {
            return _arr;
        }

        constexpr fn _Data(T* arr) noex -> void
        {
            _arr = arr;
        }

        constexpr fn _Count() const noex -> usize
        {
            return _count;
        }

        constexpr fn _Count(usize count) noex -> void
        {
            _count = count;
        }

        constexpr fn _Capacity() const noex -> usize
        {
            return _capacity;
        }

        constexpr fn _Capacity(usize cap) noex -> void
        {
            _capacity = cap;
        }

        constexpr fn _AllocMem(usize size) -> T*
        {
            return (T*)_alloc.Alloc(size);
        }

        constexpr fn _DeallocMem(T* mem) -> void
        {
            return _alloc.Dealloc(mem);
        }

        constexpr fn _CalcCapGrowth(usize required) const noex -> usize
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        constexpr fn _Swap(_DynArrImplBase& that) noex -> void
        {
            _DynArrImplBase tmp = that;
            *this = that;
            that = tmp;
        }

        constexpr fn _Move(_DynArrImplBase& that) noex -> void
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
    class DynArr: public _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>
    {
        using Base = _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>;
        using BaseImpl = _DynArrImplBase<T, TAlloc>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor DynArr() noex = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor DynArr(NullPtr) noex:
            Base{BaseImpl{ nullptr }} { }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RRangeOf<TRange, T>
        constexpr ctor DynArr(const TRange& range) noex:
            Base{BaseImpl{ nullptr }}
        {
            this->InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor DynArr(const DynArr& that) noex:
            Base{BaseImpl{ nullptr }}
        {
            this->InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// ----------------------------------------------------------------------------------------
        constexpr fn operator =(const DynArr& that) noex -> DynArr&
        {
            this->Clear();
            this->InsertBack(that);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor DynArr(DynArr&& that) noex:
            Base{BaseImpl{ MOVE(that) }} { }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        constexpr fn operator =(DynArr&& that) noex -> DynArr&
        {
            DynArr tmp = MOVE(that);
            this->_Swap(tmp);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        constexpr dtor DynArr() noex
        {
            this->Clear();
            this->Release();
        }
    };
}