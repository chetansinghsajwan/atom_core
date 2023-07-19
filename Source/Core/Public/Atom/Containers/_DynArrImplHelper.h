#pragma once
#include "_DynArrImplHelper.decl.h"
#include "_ArrImplHelper.h"
#include "Atom/Memory/ObjHelper.h"
#include "Atom/Math/Core.h"

namespace Atom
{
    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::Capacity() const noex -> usize
    {
        return _Capacity();
    }

    template <tname TImpl>
    template <tname T2, tname Req>
    cexpr fn _DynArrImplHelper<TImpl>::InsertAt(TIter pos, T2&& el) -> TMutIter
    {
        ATOM_DEBUG_EXPECTS(_ValidateIter(pos)) << TEXT("Invalid iter.");

        isize index = _FetchIndex(pos);
        ATOM_ASSERT(_ValidateIndexForInsert(index)) << IndexOutOfRangeException(
            TEXT("{pos} is out of range."), index, 0, _Count());

        return MutIter() + _InsertAt(index, fwd(el));
    }

    template <tname TImpl>
    template <tname TRange, tname Req>
    cexpr fn _DynArrImplHelper<TImpl>::InsertAt(TIter pos, const TRange& range) -> TMutIter
    {
        ATOM_DEBUG_EXPECTS(_ValidateIter(pos)) << TEXT("Invalid iter.");

        isize index = _FetchIndex(pos);
        ATOM_ASSERT(_ValidateIndexForInsert(index)) << IndexOutOfRangeException(
            TEXT("{pos} is out of range."), index, 0, _Count());

        if cexpr (_CanGetRangeSize<TRange>())
        {
            return MutIter() + _InsertAtCounted(index, range.MutIter(), _GetRangeSize(range));
        }
        else
        {
            return MutIter() + _InsertAtUncounted(index, range.MutIter(), range.IterEnd());
        }
    }

    template <tname TImpl>
    template <tname T2, tname Req>
    cexpr fn _DynArrImplHelper<TImpl>::InsertFront(T2&& el) -> TMutIter
    {
        return MutIter() + _InsertAt(0, fwd(el));
    }

    template <tname TImpl>
    template <tname TRange, tname Req>
    cexpr fn _DynArrImplHelper<TImpl>::InsertFront(const TRange& range) -> TMutIter
    {
        if cexpr (_CanGetRangeSize<TRange>())
        {
            return MutIter() + _InsertAt(0, range.MutIter(), _GetRangeSize(range));
        }
        else
        {
            return MutIter() + _InsertAt(0, range.MutIter(), range.IterEnd());
        }
    }

    template <tname TImpl>
    template <tname T2, tname Req>
    cexpr fn _DynArrImplHelper<TImpl>::InsertBack(T2&& el) -> TMutIter
    {
        return MutIter() + _InsertBack(fwd(el));
    }

    template <tname TImpl>
    template <tname TRange, tname Req>
    cexpr fn _DynArrImplHelper<TImpl>::InsertBack(const TRange& range) -> TMutIter
    {
        if cexpr (_CanGetRangeSize<TRange>())
        {
            return MutIter() + _InsertBackCounted(range.Iter(), _GetRangeSize(range));
        }
        else
        {
            return MutIter() + _InsertBackUncounted(range.Iter(), range.IterEnd());
        }
    }

    template <tname TImpl>
    template <tname T2, tname Req>
    cexpr fn _DynArrImplHelper<TImpl>::op+=(T2&& el) -> TMutIter
    {
        return InsertBack(fwd(el));
    }

    template <tname TImpl>
    template <tname TRange, tname Req>
    cexpr fn _DynArrImplHelper<TImpl>::op+=(const TRange& range) -> TMutIter
    {
        return InsertBack(range);
    }

    template <tname TImpl>
    template <tname T2>
    cexpr fn _DynArrImplHelper<TImpl>::_InsertAt(
        usize index, T2&& el) -> usize
    {
        _EnsureCapFor(1);
        _MoveRangeBack(index, 1);
        _ConstructAt(index, fwd(el));

        return index;
    }

    template <tname TImpl>
    template <tname TIter2>
    cexpr fn _DynArrImplHelper<TImpl>::_InsertAtCounted(
        usize index, TIter2 it, usize count) -> usize
    {
        if (count == 0)
            return index;

        _EnsureCapFor(count);
        _MoveRangeBack(index, count);

        // Insert new elements
        for (usize i = 0; i < count; i++)
        {
            _ConstructAt(i + index, fwd(*it++));
        }

        return index;
    }

    template <tname TImpl>
    template <tname TIter2, tname TIterEnd2>
    cexpr fn _DynArrImplHelper<TImpl>::_InsertAtUncounted(
        usize index, TIter2 begin, TIterEnd2 end) -> usize
    {
        usize rotateSize = _Count() - index;
        _InsertBackUncounted(begin, end);
        _RotateRangeBack(index, rotateSize);

        return index;
    }

    template <tname TImpl>
    template <tname  T2>
    cexpr fn _DynArrImplHelper<TImpl>::_InsertBack(T2&& el) -> usize
    {
        _EnsureCapFor(1);
        _ConstructAt(_Count(), fwd(el));
        _Count(_Count() + 1);

        return _Count() - 1;
    }

    template <tname TImpl>
    template <tname TIter2>
    cexpr fn _DynArrImplHelper<TImpl>::_InsertBackCounted(
        TIter2 it, usize count) -> usize
    {
        usize index = _Count();

        if (count == 0)
            return index;

        _EnsureCapFor(count);

        for (usize i = 0; i < count; i++)
        {
            _ConstructAt(index + i, fwd(*it++));
        }

        _Count(_Count() + count);
        return index;
    }

    template <tname TImpl>
    template <tname TIter2, tname TIterEnd2>
    cexpr fn _DynArrImplHelper<TImpl>::_InsertBackUncounted(
        TIter2 begin, TIterEnd2 end) -> usize
    {
        usize index = _Count();
        for (auto&& el : Range(begin, end))
        {
            _EnsureCapFor(1);
            _ConstructAt(_Count(), fwd(el));
            _Count(_Count() + 1);
        }

        return index;
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::RemoveAt(TIter pos) -> TMutIter
    {
        ATOM_DEBUG_EXPECTS(_ValidateIter(pos)) << TEXT("Invalid iter.");

        isize index = _FetchIndex(pos);

        ATOM_ASSERT(_ValidateIndex(index)) << IndexOutOfRangeException(
            TEXT("{pos} was out of range."), index, 0, _Count() - 1);

        return MutIter() + _RemoveAt(index);
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::RemoveRange(Range<TIter, TIterEnd> range) -> TMutIter
    {
        TIter rangeBegin = range.MutIter();
        TIterEnd rangeEnd = range.IterEnd();

        ATOM_DEBUG_EXPECTS(_ValidateIter(rangeBegin)) << TEXT("Invalid iter.");
        ATOM_DEBUG_EXPECTS(_ValidateIter(rangeEnd)) << TEXT("Invalid iter.");

        usize begin = _FetchIndex(rangeBegin);
        usize end = _FetchIndex(rangeEnd);

        ATOM_ASSERT(_ValidateIndex(begin)) << IndexOutOfRangeException(
            TEXT("{range.MutIter()} was out of range."), begin, 0, _Count() - 1);

        ATOM_ASSERT(_ValidateIndex(end)) << IndexOutOfRangeException(
            TEXT("{range.IterEnd()} was out of range."), end, 0, _Count() - 1);

        ATOM_ASSERT(begin <= end) << InvalidArgumentException(
            TEXT("Invalid range passed. {range.MutIter()} is ahead of {range.IterEnd()}"));

        return MutIter() + _RemoveRange(begin, end - begin);
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::Clear()
    {
        _RemoveRange(0, _Count() - 1);
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_RemoveAt(usize index) -> usize
    {
        _DestructAt(index);
        _MoveRangeFront(index, 1);

        return index;
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_RemoveRange(usize begin, usize count) -> usize
    {
        _DestructRange(begin, count);
        _MoveRangeFront(begin, count);

        return begin;
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::Reserve(
        usize count)
    {
        _EnsureCapFor(count);
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::Release()
    {
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_ValidateIter(
        TIter it) const noex -> bool
    {
        // TODO: Implement this.
        // return it.debugId == _iterValidDebugId;
        return true;
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_UpdateIterDebugId() noex
    {
        // _iterValidDebugId++;
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_ValidateIndex(
        isize index) const noex -> bool
    {
        return index >= 0 && (usize)index <= _Count() - 1;
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_ValidateIndexForInsert(
        isize index) const noex -> bool
    {
        return index >= 0 && index <= _Count();
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_FetchIndex(
        TIter pos) const noex -> isize
    {
        return &*pos - _Data();
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_CalcCapGrowth(
        usize required) const noex -> usize
    {
        return Math::Max(_Count() + required, _Capacity() * 2);
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_EnsureCapFor(
        usize count)
    {
        _UpdateIterDebugId();

        // We have enough capacity.
        if (_Capacity() - _Count() >= count)
            return;

        usize newCap = _CalcCapGrowth(count);
        TElem* newArr = _AllocMem(newCap);

        _MoveRangeTo(0, newArr);
        _DeallocMem(_Data());
        _Data(newArr);
        _Capacity(newCap);
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_ConstructAt(
        usize index, auto&&... args)
    {
        ObjHelper().Construct(_Data() + index, fwd(args)...);
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_DestructAt(
        usize index)
    {
        ObjHelper().Destruct(_Data() + index);
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_DestructRange(
        usize index, usize count)
    {
        RangeHelper().Destruct(Range(_Data() + index, count - index));
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_MoveRangeFront(
        usize index, usize count)
    {
        RangeHelper().FwdMoveTo(
            Range(_Data() + index, _Count() - 1 - index), 
            Range(_Data() + count, _Data() + _Count()));
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_MoveRangeBack(
        usize index, usize count)
    {
        RangeHelper().BwdMoveTo
        (
            Range(_Data() + index, _Count() - 1 - index), 
            Range(_Data() + count, _Data() + _Count())
        );
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_MoveRangeTo(
        usize index, TElem* dest)
    {
        RangeHelper().FwdMoveTo
        (
            Range(_Data() + index, _Data() + _Count()),
            Range(dest, _Count())
        );
    }

    template <tname TImpl>
    cexpr fn _DynArrImplHelper<TImpl>::_RotateRangeBack(
        usize index, usize count)
    {
        RangeHelper().RotateBwd(
            Range(_Data() + index, _Count() - 1 - index), count);
    }

    template <tname TImpl>
    template <tname TRange>
    cexpr fn _DynArrImplHelper<TImpl>::_CanGetRangeSize() noex -> bool
    {
        return RangeHelper().CanGetCount<TRange>();
    }

    template <tname TImpl>
    template <tname TRange>
    cexpr fn _DynArrImplHelper<TImpl>::_GetRangeSize(
        const TRange& range) noex -> usize
    {
        return RangeHelper().GetCount(range);
    }
}