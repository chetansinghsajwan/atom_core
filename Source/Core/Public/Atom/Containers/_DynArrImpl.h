#pragma once
#include "Atom/Math/Core.h"
#include "Atom/Memory/ObjHelper.h"
#include "_DynArrImpl.decl.h"

namespace Atom
{
    template <typename TStorage>
    constexpr auto _DynArrImpl<TStorage>::_constructAt(usize i, auto&&... args) -> void
    {
        ObjHelper().Construct(_Data() + i, fwd(args)...);
    }

    template <typename TStorage>
    constexpr auto _DynArrImpl<TStorage>::_destructAt(usize i) -> void
    {
        ObjHelper().Destruct(_Data() + i);
    }

    template <typename TStorage>
    constexpr auto _DynArrImpl<TStorage>::_destructRange(usize i, usize count) -> void
    {
        RangeHelper().Destruct(Range(_Data() + i, count - i));
    }

    template <typename TStorage>
    constexpr auto _DynArrImpl<TStorage>::_moveRangeFront(usize i, usize count) -> void
    {
        RangeHelper().FwdMoveTo(
            Range(_Data() + i, _getcount() - 1 - i), Range(_Data() + count, _Data() + _getcount()));
    }

    template <typename TStorage>
    constexpr auto _DynArrImpl<TStorage>::_moveRangeBack(usize i, usize count) -> void
    {
        RangeHelper().BwdMoveTo(
            Range(_Data() + i, _getcount() - 1 - i), Range(_Data() + count, _Data() + _getcount()));
    }

    template <typename TStorage>
    constexpr auto _DynArrImpl<TStorage>::_moveRangeTo(usize i, TElem* dest) -> void
    {
        RangeHelper().FwdMoveTo(Range(_Data() + i, _Data() + _getcount()), Range(dest, _getcount()));
    }

    template <typename TStorage>
    constexpr auto _DynArrImpl<TStorage>::_rotateRangeBack(usize i, usize count) -> void
    {
        RangeHelper().RotateBwd(Range(_Data() + i, _getcount() - 1 - i), count);
    }

    template <typename TStorage>
    template <typename TRange>
    constexpr auto _DynArrImpl<TStorage>::_canGetRangeSize() -> bool
    {
        return RangeHelper().CanGetCount<TRange>();
    }

    template <typename TStorage>
    template <typename TRange>
    constexpr auto _DynArrImpl<TStorage>::_getRangeSize(const TRange& range) -> usize
    {
        return RangeHelper().GetCount(range);
    }
}
