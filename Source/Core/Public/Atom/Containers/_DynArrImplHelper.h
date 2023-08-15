#pragma once
#include "_DynArrImplHelper.decl.h"
#include "_ArrImplHelper.h"
#include "Atom/Math/Core.h"
#include "Atom/Memory/ObjHelper.h"

namespace Atom
{
    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::Capacity() const -> usize
    {
        return _Capacity();
    }

    template <typename TImpl>
    template <typename T2, typename Req>
    constexpr auto _DynArrImplHelper<TImpl>::InsertAt(TIter pos, T2&& el) -> TMutIter
    {
        ATOM_DEBUG_EXPECTS(_ValidateIter(pos)) << "Invalid iter.";

        isize index = _FetchIndex(pos);
        ATOM_ASSERT(_ValidateIndexForInsert(index))
            << IndexOutOfRangeException("{pos} is out of range.", index, 0, _Count());

        return mutIter().next(_InsertAt(index, fwd(el)));
    }

    template <typename TImpl>
    template <typename TRange, typename Req>
    constexpr auto _DynArrImplHelper<TImpl>::InsertAt(TIter pos, const TRange& range) -> TMutIter
    {
        ATOM_DEBUG_EXPECTS(_ValidateIter(pos)) << "Invalid iter.";

        isize index = _FetchIndex(pos);
        ATOM_ASSERT(_ValidateIndexForInsert(index))
            << IndexOutOfRangeException("{pos} is out of range.", index, 0, _Count());

        if constexpr (_CanGetRangeSize<TRange>())
        {
            return mutIter().next(_InsertAtCounted(index, range.mutIter(), _GetRangeSize(range)));
        }
        else
        {
            return mutIter().next(_InsertAtUncounted(index, range.mutIter(), range.iterEnd()));
        }
    }

    template <typename TImpl>
    template <typename T2, typename Req>
    constexpr auto _DynArrImplHelper<TImpl>::InsertFront(T2&& el) -> TMutIter
    {
        return mutIter().next(_InsertAt(0, fwd(el)));
    }

    template <typename TImpl>
    template <typename TRange, typename Req>
    constexpr auto _DynArrImplHelper<TImpl>::InsertFront(const TRange& range) -> TMutIter
    {
        if constexpr (_CanGetRangeSize<TRange>())
        {
            return mutIter().next(_InsertAt(0, range.mutIter(), _GetRangeSize(range)));
        }
        else
        {
            return mutIter().next(_InsertAt(0, range.mutIter(), range.iterEnd()));
        }
    }

    template <typename TImpl>
    template <typename T2, typename Req>
    constexpr auto _DynArrImplHelper<TImpl>::InsertBack(T2&& el) -> TMutIter
    {
        return mutIter().next(_InsertBack(fwd(el)));
    }

    template <typename TImpl>
    template <typename TRange, typename Req>
    constexpr auto _DynArrImplHelper<TImpl>::InsertBack(const TRange& range) -> TMutIter
    {
        if constexpr (_CanGetRangeSize<TRange>())
        {
            return mutIter().next(_InsertBackCounted(range.iter(), _GetRangeSize(range)));
        }
        else
        {
            return mutIter().next(_InsertBackUncounted(range.iter(), range.iterEnd()));
        }
    }

    template <typename TImpl>
    template <typename T2, typename Req>
    constexpr auto _DynArrImplHelper<TImpl>::operator+=(T2&& el) -> TMutIter
    {
        return InsertBack(fwd(el));
    }

    template <typename TImpl>
    template <typename TRange, typename Req>
    constexpr auto _DynArrImplHelper<TImpl>::operator+=(const TRange& range) -> TMutIter
    {
        return InsertBack(range);
    }

    template <typename TImpl>
    template <typename T2>
    constexpr auto _DynArrImplHelper<TImpl>::_InsertAt(usize index, T2&& el) -> usize
    {
        _EnsureCapFor(1);
        _MoveRangeBack(index, 1);
        _ConstructAt(index, fwd(el));

        return index;
    }

    template <typename TImpl>
    template <typename TIter2>
    constexpr auto _DynArrImplHelper<TImpl>::_InsertAtCounted(usize index, TIter2 it, usize count)
        -> usize
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

    template <typename TImpl>
    template <typename TIter2, typename TIterEnd2>
    constexpr auto _DynArrImplHelper<TImpl>::_InsertAtUncounted(
        usize index, TIter2 begin, TIterEnd2 end) -> usize
    {
        usize rotateSize = _Count() - index;
        _InsertBackUncounted(begin, end);
        _RotateRangeBack(index, rotateSize);

        return index;
    }

    template <typename TImpl>
    template <typename T2>
    constexpr auto _DynArrImplHelper<TImpl>::_InsertBack(T2&& el) -> usize
    {
        _EnsureCapFor(1);
        _ConstructAt(_Count(), fwd(el));
        _Count(_Count() + 1);

        return _Count() - 1;
    }

    template <typename TImpl>
    template <typename TIter2>
    constexpr auto _DynArrImplHelper<TImpl>::_InsertBackCounted(TIter2 it, usize count) -> usize
    {
        usize index = _Count();

        if (count == 0)
            return index;

        _EnsureCapFor(count);

        for (usize i = 0; i < count; i++, it.next())
        {
            _ConstructAt(index + i, it.value());
        }

        _Count(_Count() + count);
        return index;
    }

    template <typename TImpl>
    template <typename TIter2, typename TIterEnd2>
    constexpr auto _DynArrImplHelper<TImpl>::_InsertBackUncounted(TIter2 begin, TIterEnd2 end)
        -> usize
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

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::RemoveAt(TIter pos) -> TMutIter
    {
        ATOM_DEBUG_EXPECTS(_ValidateIter(pos)) << "Invalid iter.";

        isize index = _FetchIndex(pos);

        ATOM_ASSERT(_ValidateIndex(index))
            << IndexOutOfRangeException("{pos} was out of range.", index, 0, _Count() - 1);

        return mutIter().next(_RemoveAt(index));
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::RemoveRange(Range<TIter, TIterEnd> range) -> TMutIter
    {
        TIter rangeBegin = range.mutIter();
        TIterEnd rangeEnd = range.iterEnd();

        ATOM_DEBUG_EXPECTS(_ValidateIter(rangeBegin)) << "Invalid iter.";
        ATOM_DEBUG_EXPECTS(_ValidateIter(rangeEnd)) << "Invalid iter.";

        usize begin = _FetchIndex(rangeBegin);
        usize end = _FetchIndex(rangeEnd);

        ATOM_ASSERT(_ValidateIndex(begin)) << IndexOutOfRangeException(
            "{range.mutIter()} was out of range.", begin, 0, _Count() - 1);

        ATOM_ASSERT(_ValidateIndex(end)) << IndexOutOfRangeException(
            "{range.iterEnd()} was out of range.", end, 0, _Count() - 1);

        ATOM_ASSERT(begin <= end) << InvalidArgumentException(
            "Invalid range passed. {range.mutIter()} is ahead of {range.iterEnd()}");

        return mutIter().next(_RemoveRange(begin, end - begin));
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::Clear() -> void
    {
        _RemoveRange(0, _Count() - 1);
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_RemoveAt(usize index) -> usize
    {
        _DestructAt(index);
        _MoveRangeFront(index, 1);

        return index;
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_RemoveRange(usize begin, usize count) -> usize
    {
        _DestructRange(begin, count);
        _MoveRangeFront(begin, count);

        return begin;
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::Reserve(usize count)
    {
        _EnsureCapFor(count);
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::Release() -> void
    {}

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_ValidateIter(TIter it) const -> bool
    {
        // TODO: Implement this.
        // return it.debugId == _iterValidDebugId;
        return true;
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_UpdateIterDebugId() -> void
    {
        // _iterValidDebugId++;
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_ValidateIndex(isize index) const -> bool
    {
        return index >= 0 && (usize)index <= _Count() - 1;
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_ValidateIndexForInsert(isize index) const -> bool
    {
        return index >= 0 && index <= _Count();
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_FetchIndex(TIter pos) const -> isize
    {
        return pos.data() - _Data();
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_CalcCapGrowth(usize required) const -> usize
    {
        // return Math::Max(_Count() + required, _Capacity() * 2);
        return required;
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_EnsureCapFor(usize count) -> void
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
    constexpr auto _DynArrImplHelper<TImpl>::_ConstructAt(usize index, auto&&... args) -> void
    {
        ObjHelper().Construct(_Data() + index, fwd(args)...);
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_DestructAt(usize index) -> void
    {
        ObjHelper().Destruct(_Data() + index);
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_DestructRange(usize index, usize count) -> void
    {
        RangeHelper().Destruct(Range(_Data() + index, count - index));
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_MoveRangeFront(usize index, usize count) -> void
    {
        RangeHelper().FwdMoveTo(Range(_Data() + index, _Count() - 1 - index),
            Range(_Data() + count, _Data() + _Count()));
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_MoveRangeBack(usize index, usize count) -> void
    {
        RangeHelper().BwdMoveTo(Range(_Data() + index, _Count() - 1 - index),
            Range(_Data() + count, _Data() + _Count()));
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_MoveRangeTo(usize index, TElem* dest) -> void
    {
        RangeHelper().FwdMoveTo(Range(_Data() + index, _Data() + _Count()), Range(dest, _Count()));
    }

    template <typename TImpl>
    constexpr auto _DynArrImplHelper<TImpl>::_RotateRangeBack(usize index, usize count) -> void
    {
        RangeHelper().RotateBwd(Range(_Data() + index, _Count() - 1 - index), count);
    }

    template <typename TImpl>
    template <typename TRange>
    constexpr auto _DynArrImplHelper<TImpl>::_CanGetRangeSize() -> bool
    {
        return RangeHelper().CanGetCount<TRange>();
    }

    template <typename TImpl>
    template <typename TRange>
    constexpr auto _DynArrImplHelper<TImpl>::_GetRangeSize(const TRange& range) -> usize
    {
        return RangeHelper().GetCount(range);
    }
}
