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
        constexpr _DynArrImplBase() = default;

        constexpr _DynArrImplBase(NullPtr):
            _arr{ nullptr }, _count{ 0 }, _capacity{ 0 }, _alloc{ } { }

    protected:
        constexpr auto _Data() const -> const T*
        {
            return _arr;
        }

        constexpr auto _Data() -> T*
        {
            return _arr;
        }

        constexpr auto _Data(T* arr) -> void
        {
            _arr = arr;
        }

        constexpr auto _Count() const -> usize
        {
            return _count;
        }

        constexpr auto _Count(usize count) -> void
        {
            _count = count;
        }

        constexpr auto _Capacity() const -> usize
        {
            return _capacity;
        }

        constexpr auto _Capacity(usize cap) -> void
        {
            _capacity = cap;
        }

        constexpr auto _AllocMem(usize size) -> T*
        {
            return (T*)_alloc.Alloc(size);
        }

        constexpr auto _DeallocMem(T* mem) -> void
        {
            return _alloc.Dealloc(mem);
        }

        constexpr auto _CalcCapGrowth(usize required) const -> usize
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        constexpr auto _Swap(_DynArrImplBase& that) -> void
        {
            _DynArrImplBase tmp = that;
            self = that;
            that = tmp;
        }

        constexpr auto _Move(_DynArrImplBase& that) -> void
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
    class DynArr : public _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>
    {
    private:
        using Base = _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>;
        using BaseImpl = _DynArrImplBase<T, TAlloc>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr() = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr(NullPtr):
            Base{BaseImpl{ nullptr }} { }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr DynArr(const TRange& range)
        requires (RRangeOf<TRange, T>):
            Base{BaseImpl{ nullptr }}
        {
            self.InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr(const DynArr& that):
            Base{BaseImpl{ nullptr }}
        {
            self.InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const DynArr& that) -> DynArr&
        {
            self.Clear();
            self.InsertBack(that);
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynArr(DynArr&& that):
            Base{BaseImpl{ mov(that) }} { }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(DynArr&& that) -> DynArr&
        {
            DynArr tmp = mov(that);
            self._Swap(tmp);
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ~DynArr()
        {
            self.Clear();
            self.Release();
        }
    };
}