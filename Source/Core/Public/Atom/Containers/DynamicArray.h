#pragma once
#include "DynamicArray.decl.h"
#include "Atom/Containers/ArrayIterator.h"
#include "Atom/Exceptions.h"
#include "Atom/Fmt/Fmt.h"
#include "Atom/Math/Core.h"

namespace Atom
{
    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray() noexcept:
        _arr{ nullptr }, _count{ 0 }, _capacity{ 0 }, _memAllocator{ },
        _iterValidDebugId{ 0 } { }

    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray(const DynamicArray& that)
    {
        InsertBack(that);
    }

    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray(DynamicArray&& that) noexcept = default;

    template <typename T, typename TAllocator>
    template <typename TRange>
    requires RFwdRange<TRange, const T>
    constexpr DynamicArray<T, TAllocator>::DynamicArray(const TRange& range):
        DynamicArray()
    {
        InsertBack(range);
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::operator += (const T& in)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        return InsertBack(in);
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::operator += (T&& in)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        return InsertBack(MOVE(in));
    }

    template <typename T, typename TAllocator>
    template <typename TRange>
    requires RFwdRange<TRange, const T>
    constexpr auto DynamicArray<T, TAllocator>::operator += (const TRange& range)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        return InsertBack(range);
    }

    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::~DynamicArray() { }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Data() noexcept
        -> T*
    {
        return _arr;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Data() const noexcept
        -> const T* 
    {
        return _arr;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Count() const noexcept
        -> usize
    {
        return _count;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::IsEmpty() const noexcept
        -> bool
    {
        return Count() == 0;
    }

    template <typename T, typename TAllocator>
    template <typename U>
    requires RSameAs<TTI::TRemoveCVRef<U>, T>
    constexpr auto DynamicArray<T, TAllocator>::InsertAt(TIter pos, U&& el)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        ATOM_DEBUG_EXPECTS(_ValidateIter(pos)) << TEXT("Invalid iter.");

        isize index = _FetchIndex(pos);
        ATOM_ASSERT(_ValidateIndexForInsertAt(index)) << IndexOutOfRangeException(
            TEXT("{pos} is out of range."), index, 0, _count);

        return Begin() + _InsertAt(index, FORWARD(el));
    }

    template <typename T, typename TAllocator>
    template <typename TRange>
    requires RFwdRange<TRange, const T>
    constexpr auto DynamicArray<T, TAllocator>::InsertAt(TIter pos, const TRange& range)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        ATOM_DEBUG_EXPECTS(_ValidateIter(pos)) << TEXT("Invalid iter.");

        isize index = _FetchIndex(pos);
        ATOM_ASSERT(_ValidateIndexForInsertAt(index)) << IndexOutOfRangeException(
            TEXT("{pos} is out of range."), index, 0, _count);

        if constexpr (_CanGetRangeSize<TRange>())
        {
            return Begin() + _InsertAtCounted(index, range.Begin(), _GetRangeSize(range));
        }
        else
        {
            return Begin() + _InsertAtUncounted(index, range.Begin(), range.End());
        }
    }

    template <typename T, typename TAllocator>
    template <typename U>
    requires RSameAs<TTI::TRemoveCVRef<U>, T>
    constexpr auto DynamicArray<T, TAllocator>::InsertFront(U&& el)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        return Begin() + _InsertAt(0, FORWARD(el));
    }

    template <typename T, typename TAllocator>
    template <typename TRange>
    requires RFwdRange<TRange, const T>
    constexpr auto DynamicArray<T, TAllocator>::InsertFront(const TRange& range)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        if constexpr (_CanGetRangeSize<TRange>())
        {
            return Begin() + _InsertAt(0, range.Begin(), _GetRangeSize(range));
        }
        else
        {
            return Begin() + _InsertAt(0, range.Begin(), range.End());
        }
    }

    template <typename T, typename TAllocator>
    template <typename U>
    requires RSameAs<TTI::TRemoveCVRef<U>, T>
    constexpr auto DynamicArray<T, TAllocator>::InsertBack(U&& el)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        return Begin() + _InsertBack(FORWARD(el));
    }

    template <typename T, typename TAllocator>
    template <typename TRange>
    requires RFwdRange<TRange, const T>
    constexpr auto DynamicArray<T, TAllocator>::InsertBack(const TRange& range)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        if constexpr (_CanGetRangeSize<TRange>())
        {
            return Begin() + _InsertBackCounted(range.Begin(), _GetRangeSize(range));
        }
        else
        {
            return Begin() + _InsertBackUncounted(range.Begin(), range.End());
        }
    }

    template <typename T, typename TAllocator>
    template <typename U>
    constexpr usize DynamicArray<T, TAllocator>::_InsertAt(
        usize index, U&& el)
    {
        _EnsureCapFor(1);
        _MoveRangeBack(index, 1);
        _ConstructAt(index, FORWARD(el));

        return index;
    }

    template <typename T, typename TAllocator>
    template <typename TOtherIter>
    constexpr usize DynamicArray<T, TAllocator>::_InsertAtCounted(
        usize index, TOtherIter it, usize count)
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

    template <typename T, typename TAllocator>
    template <typename TOtherIter, typename TOtherIterEnd>
    constexpr usize DynamicArray<T, TAllocator>::_InsertAtUncounted(
        usize index, TOtherIter begin, TOtherIterEnd end)
    {
        usize rotateSize = _count - index;
        _InsertBackUncounted(begin, end);
        _RotateRangeBack(index, rotateSize);

        return index;
    }

    template <typename T, typename TAllocator>
    template <typename  U>
    constexpr usize DynamicArray<T, TAllocator>::_InsertBack(U&& el)
    {
        _EnsureCapFor(1);
        _ConstructAt(_count++, FORWARD(el));

        return _count - 1;
    }

    template <typename T, typename TAllocator>
    template <typename TOtherIter>
    constexpr usize DynamicArray<T, TAllocator>::_InsertBackCounted(
        TOtherIter it, usize count)
    {
        usize index = _count;

        if (count == 0)
            return index;

        _EnsureCapFor(count);

        for (usize i = 0; i < count; i++)
        {
            _ConstructAt(_count++, FORWARD(*it++));
        }

        return index;
    }

    template <typename T, typename TAllocator>
    template <typename TOtherIter, typename TOtherIterEnd>
    constexpr usize DynamicArray<T, TAllocator>::_InsertBackUncounted(
        TOtherIter begin, TOtherIterEnd end)
    {
        usize index = _count;
        for (auto&& el : Range(begin, end))
        {
            _EnsureCapFor(1);
            _ConstructAt(_count++, FORWARD(el));
        }

        return index;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::RemoveAt(TConstIter pos)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        ATOM_DEBUG_EXPECTS(_ValidateIter(pos)) << TEXT("Invalid iter.");

        isize index = _FetchIndex(pos);

        ATOM_ASSERT(_ValidateIndex(index)) << IndexOutOfRangeException(
            TEXT("{pos} was out of range."), index, 0, _count - 1);

        return Begin() + _RemoveAt(index);
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::RemoveRange(Range<TConstIter, TConstIterEnd> range)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        TConstIter rangeBegin = range.Begin();
        TConstIterEnd rangeEnd = range.End();

        ATOM_DEBUG_EXPECTS(_ValidateIter(rangeBegin)) << TEXT("Invalid iter.");
        ATOM_DEBUG_EXPECTS(_ValidateIter(rangeEnd)) << TEXT("Invalid iter.");

        usize begin = _FetchIndex(rangeBegin);
        usize end = _FetchIndex(rangeEnd);

        ATOM_ASSERT(_ValidateIndex(begin)) << IndexOutOfRangeException(
            TEXT("{range.Begin()} was out of range."), begin, 0, _count - 1);

        ATOM_ASSERT(_ValidateIndex(end)) << IndexOutOfRangeException(
            TEXT("{range.End()} was out of range."), end, 0, _count - 1);

        ATOM_ASSERT(begin <= end) << InvalidArgumentException(
            TEXT("Invalid range passed. {range.Begin()} is ahead of {range.End()}"));

        return Begin() + _RemoveRange(begin, end - begin);
    }

    template <typename T, typename TAllocator>
    constexpr usize DynamicArray<T, TAllocator>::_RemoveAt(usize index)
    {
        _DestructAt(index);
        _MoveRangeFront(index, 1);

        return index;
    }

    template <typename T, typename TAllocator>
    constexpr usize DynamicArray<T, TAllocator>::_RemoveRange(usize begin, usize count)
    {
        _DestructRange(begin, count);
        _MoveRangeFront(begin, count);

        return begin;
    }

    template <typename T, typename TAllocator>
    template <typename TRange>
    requires RFwdRange<TRange, const T>
    constexpr auto DynamicArray<T, TAllocator>::Remove(const TRange& range)
        -> usize
    {
        usize count = 0;

        for (const auto& element : range)
        {
            for (auto it = Begin(); it != End(); it++)
            {
                if (*it == element)
                {
                    it = RemoveAt(it);
                    count++;
                }
            }
        }

        return count;
    }

    template <typename T, typename TAllocator>
    template <RInvokable<bool(const T&)> TFilter>
    auto DynamicArray<T, TAllocator>::RemoveIf(TFilter&& filter)
        -> usize
    {
        usize count = 0;

        for (auto it = Begin(); it != End(); it++)
        {
            if (filter(*it))
            {
                it = RemoveAt(it);
                count++;
            }
        }

        return count;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Remove(const T& in)
        -> bool
    {
        for (auto it = Begin(); it != End(); it++)
        {
            if (*it == in)
            {
                RemoveAt(it);
                return true;
            }
        }

        return false;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Contains(const T& in) const noexcept
        -> bool
    {
        for (const T& el : *this)
        {
            if (el == in)
            {
                return true;
            }
        }

        return false;
    }

    template <typename T, typename TAllocator>
    constexpr void DynamicArray<T, TAllocator>::Reserve(
        usize count)
    {
        _EnsureCapFor(count);
    }

    template <typename T, typename TAllocator>
    constexpr bool DynamicArray<T, TAllocator>::_ValidateIter(
        TConstIter it) const noexcept
    {
        return it.debugId != _iterValidDebugId;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::_ValidateIndex(
        isize index) const noexcept -> bool
    {
        return index >= 0 && index <= _count - 1;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::_ValidateIndexForInsertAt(
        isize index) const noexcept -> bool
    {
        return index >= 0 && index <= _count;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::_FetchIndex(
        TConstIter pos) const noexcept -> isize
    {
        isize index = Data() - &*pos;

        if (index < 0) return nullpos;
        if (index > _count) return nullpos;
        return index;
    }

    template <typename T, typename TAllocator>
    constexpr usize DynamicArray<T, TAllocator>::_CalcCapGrowth(
        usize required) const noexcept
    {
        return Math::Max(_count + required, _capacity * 2);
    }

    template <typename T, typename TAllocator>
    constexpr void DynamicArray<T, TAllocator>::_EnsureCapFor(
        usize count)
    {
        // We have enough capacity.
        if (_capacity - _count >= count)
            return;

        DynamicArray<T> tmp(_CalcCapGrowth(count));
        _MoveRange(_arr, tmp._arr, _count);
        SWAP(_count, tmp._count);
        SWAP(_iterValidDebugId, tmp._iterValidDebugId);

        _Swap(tmp);
    }

    template <typename T, typename TAllocator>
    constexpr void DynamicArray<T, TAllocator>::_ConstructAt(
        usize index, auto&&... args)
    {
        new (_arr + index) T(FORWARD(args)...);
    }

    template <typename T, typename TAllocator>
    constexpr void DynamicArray<T, TAllocator>::_DestructAt(
        usize index)
    {
        if constexpr (std::is_trivially_destructible_v<T>)
            return;

        (Data() + index)->T::~T();
    }

    template <typename T, typename TAllocator>
    constexpr void DynamicArray<T, TAllocator>::_DestructRange(
        usize begin, usize end)
    {
        if constexpr (std::is_trivially_destructible_v<T>)
            return;

        for (usize i = begin; i < end; i++)
        {
            (Data() + i)->T::~T();
        }
    }

    struct MemHelper
    {
        constexpr void Copy(const void* src, void* dest, usize size) const noexcept
        {
        }

        constexpr void CopyFwd(const void* src, usize steps, usize size) const noexcept
        {
        }
    };

    template <typename T>
    struct ObjHelper
    {
        constexpr void Move(const T* src, T* dest, usize size) const noexcept
        {
        }

        constexpr void MoveFwd(const T* src, usize steps, usize size) const noexcept
        {
            if constexpr (std::is_trivially_move_constructible_v<T>)
            {
                MemHelper().CopyFwd(Data() + index, count, Count() - index);
                return;
            }

            T* arr = Data();
            for (usize i = _count; i > index; i--)
            {
                arr[i - 1 + count] = MOVE(arr[i - 1]);
            }
        }
    };

    template <typename T, typename TAllocator>
    constexpr void DynamicArray<T, TAllocator>::_MoveRangeFront(
        usize index, usize count)
    {
        ATOM_DEBUG_EXPECTS(count > 0);

        ObjHelper<T>().MoveFwd(Data() + index, Count() - index, count);

        if constexpr (std::is_trivially_move_constructible_v<T>)
        {
            MemHelper().CopyFwd(Data() + index, count, Count() - index);
            return;
        }

        T* arr = Data();
        for (usize i = _count; i > index; i--)
        {
            arr[i - 1 + count] = MOVE(arr[i - 1]);
        }
    }

    template <typename T, typename TAllocator>
    constexpr void DynamicArray<T, TAllocator>::_MoveRangeBack(
        usize index, usize count)
    {
        if constexpr (std::is_trivially_move_constructible_v<T>)
        {
            for (usize i = count * sizeof(T); i > 0; i--)
            {
            }

            return;
        }

        T* arr = Data();
        for (usize i = _count - 1; i > index; i--)
        {
            arr[i + count] = MOVE(arr[i]);
        }
    }

    template <typename T, typename TAllocator>
    constexpr void DynamicArray<T, TAllocator>::_RotateRangeBack(
        usize index, usize count)
    {
        // TODO: Refactor this.
        // NOTE: Keep std implementation, MSVC uses vectorization for this.
        std::rotate(Data() + index, Data() + index + count, Data() + _count - 1);
    }

    template <typename T, typename TAllocator>
    template <typename TRange>
    static constexpr auto DynamicArray<T, TAllocator>::_CanGetRangeSize()
        noexcept -> bool
    {
        return RMultiPassRange<TRange, T>;
    }

    template <typename T, typename TAllocator>
    template <typename TRange>
    static constexpr usize DynamicArray<T, TAllocator>::_GetRangeSize(
        const TRange& range) noexcept
    {
        if constexpr (RFwdJumpRange<TRange, T>)
        {
            return range.Size();
        }

        usize size = 0;
        for (auto&& el : range) size++;

        return size;
    }
}