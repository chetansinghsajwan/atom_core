#pragma once
#include "_DynArrImplHelper.decl.h"
#include "_ArrImplHelper.h"
#include "Atom/Memory/ObjHelper.h"
#include "Atom/Math/Core.h"

namespace Atom
{
    template <typename TImpl>
    cexpr auto _DynArrImplHelper<TImpl>::Capacity() const noex
        -> usize
    {
        return _Capacity();
    }

    template <typename TImpl>
    template <typename U, typename Req>
    cexpr auto _DynArrImplHelper<TImpl>::InsertAt(TIter pos, U&& el)
        -> typename _DynArrImplHelper<TImpl>::TMutIter
    {
        ATOM_DEBUG_EXPECTS(_ValidateIter(pos)) << TEXT("Invalid iter.");

        isize index = _FetchIndex(pos);
        ATOM_ASSERT(_ValidateIndexForInsert(index)) << IndexOutOfRangeException(
            TEXT("{pos} is out of range."), index, 0, _Count());

        return MutIter() + _InsertAt(index, FORWARD(el));
    }

    template <typename TImpl>
    template <typename TRange, typename Req>
    cexpr auto _DynArrImplHelper<TImpl>::InsertAt(TIter pos, const TRange& range)
        -> typename _DynArrImplHelper<TImpl>::TMutIter
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

    template <typename TImpl>
    template <typename U, typename Req>
    cexpr auto _DynArrImplHelper<TImpl>::InsertFront(U&& el)
        -> typename _DynArrImplHelper<TImpl>::TMutIter
    {
        return MutIter() + _InsertAt(0, FORWARD(el));
    }

    template <typename TImpl>
    template <typename TRange, typename Req>
    cexpr auto _DynArrImplHelper<TImpl>::InsertFront(const TRange& range)
        -> typename _DynArrImplHelper<TImpl>::TMutIter
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

    template <typename TImpl>
    template <typename U, typename Req>
    cexpr auto _DynArrImplHelper<TImpl>::InsertBack(U&& el)
        -> typename _DynArrImplHelper<TImpl>::TMutIter
    {
        return MutIter() + _InsertBack(FORWARD(el));
    }

    template <typename TImpl>
    template <typename TRange, typename Req>
    cexpr auto _DynArrImplHelper<TImpl>::InsertBack(const TRange& range)
        -> typename _DynArrImplHelper<TImpl>::TMutIter
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

    template <typename TImpl>
    template <typename U, typename Req>
    cexpr auto _DynArrImplHelper<TImpl>::operator +=(U&& el)
        -> typename _DynArrImplHelper<TImpl>::TMutIter
    {
        return InsertBack(FORWARD(el));
    }

    template <typename TImpl>
    template <typename TRange, typename Req>
    cexpr auto _DynArrImplHelper<TImpl>::operator +=(const TRange& range)
        -> typename _DynArrImplHelper<TImpl>::TMutIter
    {
        return InsertBack(range);
    }

    template <typename TImpl>
    template <typename U>
    cexpr usize _DynArrImplHelper<TImpl>::_InsertAt(
        usize index, U&& el)
    {
        _EnsureCapFor(1);
        _MoveRangeBack(index, 1);
        _ConstructAt(index, FORWARD(el));

        return index;
    }

    template <typename TImpl>
    template <typename UIter>
    cexpr usize _DynArrImplHelper<TImpl>::_InsertAtCounted(
        usize index, UIter it, usize count)
    {
        if (count == 0)
            return index;

        _EnsureCapFor(count);
        _MoveRangeBack(index, count);

        // Insert new elements
        for (usize i = 0; i < count; i++)
        {
            _ConstructAt(i + index, FORWARD(*it++));
        }

        return index;
    }

    template <typename TImpl>
    template <typename UIter, typename UIterEnd>
    cexpr usize _DynArrImplHelper<TImpl>::_InsertAtUncounted(
        usize index, UIter begin, UIterEnd end)
    {
        usize rotateSize = _Count() - index;
        _InsertBackUncounted(begin, end);
        _RotateRangeBack(index, rotateSize);

        return index;
    }

    template <typename TImpl>
    template <typename  U>
    cexpr usize _DynArrImplHelper<TImpl>::_InsertBack(U&& el)
    {
        _EnsureCapFor(1);
        _ConstructAt(_Count(), FORWARD(el));
        _Count(_Count() + 1);

        return _Count() - 1;
    }

    template <typename TImpl>
    template <typename UIter>
    cexpr usize _DynArrImplHelper<TImpl>::_InsertBackCounted(
        UIter it, usize count)
    {
        usize index = _Count();

        if (count == 0)
            return index;

        _EnsureCapFor(count);

        for (usize i = 0; i < count; i++)
        {
            _ConstructAt(index + i, FORWARD(*it++));
        }

        _Count(_Count() + count);
        return index;
    }

    template <typename TImpl>
    template <typename UIter, typename UIterEnd>
    cexpr usize _DynArrImplHelper<TImpl>::_InsertBackUncounted(
        UIter begin, UIterEnd end)
    {
        usize index = _Count();
        for (auto&& el : Range(begin, end))
        {
            _EnsureCapFor(1);
            _ConstructAt(_Count(), FORWARD(el));
            _Count(_Count() + 1);
        }

        return index;
    }

    template <typename TImpl>
    cexpr auto _DynArrImplHelper<TImpl>::RemoveAt(TIter pos)
        -> typename _DynArrImplHelper<TImpl>::TMutIter
    {
        ATOM_DEBUG_EXPECTS(_ValidateIter(pos)) << TEXT("Invalid iter.");

        isize index = _FetchIndex(pos);

        ATOM_ASSERT(_ValidateIndex(index)) << IndexOutOfRangeException(
            TEXT("{pos} was out of range."), index, 0, _Count() - 1);

        return MutIter() + _RemoveAt(index);
    }

    template <typename TImpl>
    cexpr auto _DynArrImplHelper<TImpl>::RemoveRange(Range<TIter, TIterEnd> range)
        -> typename _DynArrImplHelper<TImpl>::TMutIter
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

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::Clear()
    {
        _RemoveRange(0, _Count() - 1);
    }

    template <typename TImpl>
    cexpr usize _DynArrImplHelper<TImpl>::_RemoveAt(usize index)
    {
        _DestructAt(index);
        _MoveRangeFront(index, 1);

        return index;
    }

    template <typename TImpl>
    cexpr usize _DynArrImplHelper<TImpl>::_RemoveRange(usize begin, usize count)
    {
        _DestructRange(begin, count);
        _MoveRangeFront(begin, count);

        return begin;
    }

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::Reserve(
        usize count)
    {
        _EnsureCapFor(count);
    }

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::Release()
    {
    }

    template <typename TImpl>
    cexpr bool _DynArrImplHelper<TImpl>::_ValidateIter(
        TIter it) const noex
    {
        // TODO: Implement this.
        // return it.debugId == _iterValidDebugId;
        return true;
    }

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::_UpdateIterDebugId()
        noex
    {
        // _iterValidDebugId++;
    }

    template <typename TImpl>
    cexpr auto _DynArrImplHelper<TImpl>::_ValidateIndex(
        isize index) const noex -> bool
    {
        return index >= 0 && (usize)index <= _Count() - 1;
    }

    template <typename TImpl>
    cexpr auto _DynArrImplHelper<TImpl>::_ValidateIndexForInsert(
        isize index) const noex -> bool
    {
        return index >= 0 && index <= _Count();
    }

    template <typename TImpl>
    cexpr auto _DynArrImplHelper<TImpl>::_FetchIndex(
        TIter pos) const noex -> isize
    {
        return &*pos - _Data();
    }

    template <typename TImpl>
    cexpr usize _DynArrImplHelper<TImpl>::_CalcCapGrowth(
        usize required) const noex
    {
        return Math::Max(_Count() + required, _Capacity() * 2);
    }

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::_EnsureCapFor(
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

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::_ConstructAt(
        usize index, auto&&... args)
    {
        ObjHelper().Construct(_Data() + index, FORWARD(args)...);
    }

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::_DestructAt(
        usize index)
    {
        ObjHelper().Destruct(_Data() + index);
    }

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::_DestructRange(
        usize index, usize count)
    {
        RangeHelper().Destruct(Range(_Data() + index, count - index));
    }

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::_MoveRangeFront(
        usize index, usize count)
    {
        RangeHelper().FwdMoveTo(
            Range(_Data() + index, _Count() - 1 - index), 
            Range(_Data() + count, _Data() + _Count()));
    }

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::_MoveRangeBack(
        usize index, usize count)
    {
        RangeHelper().BwdMoveTo
        (
            Range(_Data() + index, _Count() - 1 - index), 
            Range(_Data() + count, _Data() + _Count())
        );
    }

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::_MoveRangeTo(
        usize index, TElem* dest)
    {
        RangeHelper().FwdMoveTo
        (
            Range(_Data() + index, _Data() + _Count()),
            Range(dest, _Count())
        );
    }

    template <typename TImpl>
    cexpr void _DynArrImplHelper<TImpl>::_RotateRangeBack(
        usize index, usize count)
    {
        RangeHelper().RotateBwd(
            Range(_Data() + index, _Count() - 1 - index), count);
    }

    template <typename TImpl>
    template <typename TRange>
    cexpr auto _DynArrImplHelper<TImpl>::_CanGetRangeSize()
        noex -> bool
    {
        return RangeHelper().CanGetCount<TRange>();
    }

    template <typename TImpl>
    template <typename TRange>
    cexpr usize _DynArrImplHelper<TImpl>::_GetRangeSize(
        const TRange& range) noex
    {
        return RangeHelper().GetCount(range);
    }
}