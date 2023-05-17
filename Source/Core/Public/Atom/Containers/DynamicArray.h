#pragma once
#include "DynamicArray.decl.h"
#include "Atom/Containers/ArrayIterator.h"

namespace Atom
{
    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray() noexcept:
        _vector{ } { }

    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray(const DynamicArray& that):
        _vector{ that._vector } { }

    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray(DynamicArray&& that) noexcept:
        _vector{ MOVE(that._vector) } { }

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
        return _vector.data();
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Data() const noexcept
        -> const T* 
    {
        return _vector.data();
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Count() const noexcept
        -> usize
    {
        return _vector.size();
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::IsEmpty() const noexcept
        -> bool
    {
        return Count() == 0;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::InsertAt(TIter pos, const T& in)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        usize index = &*pos - _vector.data();
        _vector.insert(_vector.begin() + index, in);
        return TIter{ _vector.data() + index };
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::InsertAt(TIter pos, T&& in)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        usize index = &*pos - _vector.data();
        _vector.insert(_vector.begin() + index, MOVE(in));
        return TIter{ _vector.data() + index };
    }

    template <typename T, typename TAllocator>
    template <typename TRange>
    requires RFwdRange<TRange, const T>
    constexpr auto DynamicArray<T, TAllocator>::InsertAt(TIter pos, const TRange& range)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        return TIter{ };
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::InsertFront(const T& in)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        return InsertAt(Begin(), in);
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::InsertFront(T&& in)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        return InsertAt(Begin(), MOVE(in));
    }

    template <typename T, typename TAllocator>
    template <typename TRange>
    requires RFwdRange<TRange, const T>
    constexpr auto DynamicArray<T, TAllocator>::InsertFront(const TRange& range)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        return InsertAt(Begin(), range);
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::InsertBack(const T& in)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        _vector.push_back(in);
        return End() - 1;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::InsertBack(T&& in)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        _vector.push_back(MOVE(in));
        return End() - 1;
    }

    template <typename T, typename TAllocator>
    template <typename TRange>
    requires RFwdRange<TRange, const T>
    constexpr auto DynamicArray<T, TAllocator>::InsertBack(const TRange& range)
        -> typename DynamicArray<T, TAllocator>::TIter
    {
        for (const T& e : range)
        {
            _vector.push_back(e);
        }

        return TIter();
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
            for (auto it = _vector.begin(); it != _vector.end(); it++)
            {
                if (*it == element)
                {
                    it = _vector.erase(it);
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

        for (auto it = _vector.begin(); it != _vector.end(); it++)
        {
            if (filter(*it))
            {
                it = _vector.erase(it);
                count++;
            }
        }

        return count;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Remove(TIter it)
        -> bool
    {
        auto std_it = _vector.begin() + (it.begin() - _vector.data());
        return _vector.erase(std_it) != _vector.end();
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Remove(const T& in)
        -> bool
    {
        for (auto it = _vector.begin(); it != _vector.end(); it++)
        {
            if (*it == in)
            {
                _vector.erase(it);
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
    constexpr auto DynamicArray<T, TAllocator>::Reserve(usize size)
        -> void
    {
        _vector.reserve(size);
    }
}