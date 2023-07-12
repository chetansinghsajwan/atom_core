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
        constexpr _DynArrImplBase() noex = default;

        constexpr _DynArrImplBase(NullPtr) noex:
            _arr{ nullptr }, _count{ 0 }, _capacity{ 0 }, _alloc{ } { }

    protected:
        constexpr const T* _Data() const noex
        {
            return _arr;
        }

        constexpr T* _Data() noex
        {
            return _arr;
        }

        constexpr void _Data(T* arr) noex
        {
            _arr = arr;
        }

        constexpr usize _Count() const noex
        {
            return _count;
        }

        constexpr void _Count(usize count) noex
        {
            _count = count;
        }

        constexpr usize _Capacity() const noex
        {
            return _capacity;
        }

        constexpr void _Capacity(usize cap) noex
        {
            _capacity = cap;
        }

        constexpr T* _AllocMem(usize size)
        {
            return (T*)_alloc.Alloc(size);
        }

        constexpr void _DeallocMem(T* mem)
        {
            return _alloc.Dealloc(mem);
        }

        constexpr usize _CalcCapGrowth(usize required) const noex
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        constexpr void _Swap(_DynArrImplBase& that) noex
        {
            _DynArrImplBase tmp = that;
            *this = that;
            that = tmp;
        }

        constexpr void _Move(_DynArrImplBase& that) noex
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
        constexpr DynArr() noex = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr(NullPtr) noex:
            Base{BaseImpl{ nullptr }} { }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RRangeOf<TRange, T>
        constexpr DynArr(const TRange& range) noex:
            Base{BaseImpl{ nullptr }}
        {
            this->InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr(const DynArr& that) noex:
            Base{BaseImpl{ nullptr }}
        {
            this->InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr& operator =(const DynArr& that) noex
        {
            this->Clear();
            this->InsertBack(that);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr(DynArr&& that) noex:
            Base{BaseImpl{ MOVE(that) }} { }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr& operator =(DynArr&& that) noex
        {
            DynArr tmp = MOVE(that);
            this->_Swap(tmp);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ~DynArr() noex
        {
            this->Clear();
            this->Release();
        }
    };
}