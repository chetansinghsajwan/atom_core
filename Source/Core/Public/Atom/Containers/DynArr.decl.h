#pragma once
#include "_DynArrImplHelper.decl.h"
#include "Atom/Memory/DefaultMemAllocator.h"
#include "Atom/Invokable/Invokable.h"
// #include "Atom/Math/Core.h"

namespace Atom
{
    template <typename T, typename TAlloc>
    class _DynArrImplBase
    {
        pubm using TElem = T;

        pubm cexpr _DynArrImplBase() noex = default;

        pubm cexpr _DynArrImplBase(NullPtr) noex:
            _arr{ nullptr }, _count{ 0 }, _capacity{ 0 }, _alloc{ } { }

        prom cexpr const T* _Data() const noex
        {
            return _arr;
        }

        prom cexpr T* _Data() noex
        {
            return _arr;
        }

        prom cexpr void _Data(T* arr) noex
        {
            _arr = arr;
        }

        prom cexpr usize _Count() const noex
        {
            return _count;
        }

        prom cexpr void _Count(usize count) noex
        {
            _count = count;
        }

        prom cexpr usize _Capacity() const noex
        {
            return _capacity;
        }

        prom cexpr void _Capacity(usize cap) noex
        {
            _capacity = cap;
        }

        prom cexpr T* _AllocMem(usize size)
        {
            return (T*)_alloc.Alloc(size);
        }

        prom cexpr void _DeallocMem(T* mem)
        {
            return _alloc.Dealloc(mem);
        }

        prom cexpr usize _CalcCapGrowth(usize required) const noex
        {
            // return Math::Max(_Count() + required, _Capacity() * 2);
            return required;
        }

        prom cexpr void _Swap(_DynArrImplBase& that) noex
        {
            _DynArrImplBase tmp = that;
            *this = that;
            that = tmp;
        }

        prom cexpr void _Move(_DynArrImplBase& that) noex
        {
            *this = that;
            that = _DynArrImplBase(nullptr);
        }

        pubm T* _arr;
        pubm usize _count;
        pubm usize _capacity;
        pubm TAlloc _alloc;
    };

    template <typename T, typename TAlloc = DefaultMemAllocator>
    class DynArr: pub _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>
    {
        using _Impl = _DynArrImplHelper<_DynArrImplBase<T, TAlloc>>;
        using _ImplBase = _DynArrImplBase<T, TAlloc>;

        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr DynArr() noex = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr DynArr(NullPtr) noex:
            _Impl{_ImplBase{ nullptr }} { }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        pubm template <typename TRange>
        requires RRangeOf<TRange, T>
        cexpr DynArr(const TRange& range) noex:
            _Impl{_ImplBase{ nullptr }}
        {
            this->InsertBack(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyCtor.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr DynArr(const DynArr& that) noex:
            _Impl{_ImplBase{ nullptr }}
        {
            this->InsertBack(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyOper.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr DynArr& operator =(const DynArr& that) noex
        {
            this->Clear();
            this->InsertBack(that);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveCtor.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr DynArr(DynArr&& that) noex:
            _Impl{_ImplBase{ MOVE(that) }} { }

        /// ----------------------------------------------------------------------------------------
        /// MoveOper.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr DynArr& operator =(DynArr&& that) noex
        {
            DynArr tmp = MOVE(that);
            this->_Swap(tmp);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Dtor.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr ~DynArr() noex
        {
            this->Clear();
            this->Release();
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// @TOOD: Move to Removeable section.
    /// --------------------------------------------------------------------------------------------
    template <typename T, typename TPred>
    requires RInvokable<TPred, bool(const T& el)>
    usize UfcsRemoveIf(DynArr<T>& range, TPred&& pred)
    {
        usize count = 0;
        for (auto it = range.Iter(); it != range.IterEnd(); it++)
        {
            if (pred(*it))
            {
                it = range.RemoveAt(it);
                count++;
            }
        };

        return count;
    }
}