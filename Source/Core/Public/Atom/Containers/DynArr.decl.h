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
        constexpr _DynArrImplBase() noexcept = default;

        constexpr _DynArrImplBase(NullPtr) noexcept:
            _arr{ nullptr }, _count{ 0 }, _capacity{ 0 }, _alloc{ } { }

    protected:
        constexpr const T* _Data() const noexcept
        {
            return _arr;
        }

        constexpr T* _Data() noexcept
        {
            return _arr;
        }

        constexpr void _Data(T* arr) noexcept
        {
            _arr = arr;
        }

        constexpr usize _Count() const noexcept
        {
            return _count;
        }

        constexpr void _Count(usize count) noexcept
        {
            _count = count;
        }

        constexpr usize _Capacity() const noexcept
        {
            return _capacity;
        }

        constexpr void _Capacity(usize cap) noexcept
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

        constexpr usize _CalcCapGrowth(usize required) const noexcept
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        constexpr void _Swap(_DynArrImplBase& that) noexcept
        {
            _DynArrImplBase tmp = that;
            *this = that;
            that = tmp;
        }

        constexpr void _Move(_DynArrImplBase& that) noexcept
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

    template <typename T, typename TAlloc = DefaultMemAllocator>
    class DynArr: public _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>
    {
        using Base = _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>;
        using BaseImpl = _DynArrImplBase<T, TAlloc>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr() noexcept = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr(NullPtr) noexcept:
            Base{BaseImpl{ nullptr }} { }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRangeOf<TRange, T>
        constexpr DynArr(const TRange& range) noexcept:
            Base{BaseImpl{ nullptr }}
        {
            this->InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr(const DynArr& that) noexcept:
            Base{BaseImpl{ nullptr }}
        {
            this->InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr& operator =(const DynArr& that) noexcept
        {
            this->Clear();
            this->InsertBack(that);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr(DynArr&& that) noexcept:
            Base{BaseImpl{ MOVE(that) }} { }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr& operator =(DynArr&& that) noexcept
        {
            DynArr tmp = MOVE(that);
            this->_Swap(tmp);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ~DynArr() noexcept
        {
            this->Clear();
            this->Release();
        }
    };
}