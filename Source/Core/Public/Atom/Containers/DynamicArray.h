#pragma once
#include "DynamicArray.decl.h"
#include "Atom/Containers/ArrayIterator.h"

namespace Atom
{
    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray() noexcept:
        _vector{ } { }

    template <typename T, typename TAllocator>
    template <typename TIt>
    requires RFwdIt<TIt, const T>
    constexpr DynamicArray<T, TAllocator>::DynamicArray(TIt it):
        DynamicArray()
    {
        InsertBack(MOVE(it));
    }

    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray(const STD_TVector& vec) noexcept:
        _vector{ vec } { }

    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray(STD_TVector&& vec) noexcept:
        _vector{ MOVE(vec) } { }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::operator += (const T& in)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        return InsertBack(in);
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::operator += (T&& in)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        return InsertBack(MOVE(in));
    }

    template <typename T, typename TAllocator>
    template <typename TIt>
    requires RFwdIt<TIt, const T>
    constexpr auto DynamicArray<T, TAllocator>::operator += (TIt it)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        return InsertBack(MOVE(it));
    }

    template <typename T, typename TAllocator>
    template <typename TRange>
    requires RFwdRange<TRange, const T>
    constexpr auto DynamicArray<T, TAllocator>::operator += (const TRange& range)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        return InsertBack(range.Iterator());
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
    constexpr auto DynamicArray<T, TAllocator>::Iterator() noexcept
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        return TIterator(_vector.data(), _vector.size());
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Iterator() const noexcept
        -> typename DynamicArray<T, TAllocator>::TConstIterator
    {
        return TConstIterator(_vector.data(), _vector.size());
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Insert(TIterator pos, const T& in)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        usize index = _vector.data() - pos.begin();
        auto out = _vector.insert(_vector.begin() + index, in);
        return TIterator{ _vector.data() + index, Count() - index };
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Insert(TIterator pos, T&& in)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        usize index = _vector.data() - pos.begin();
        auto out = _vector.insert(_vector.begin() + index, MOVE(in));
        return TIterator{ _vector.data() + index, Count() - index };
    }

    template <typename T, typename TAllocator>
    template <RFwdIt<T> TIt>
    constexpr auto DynamicArray<T, TAllocator>::Insert(TIterator pos, TIt it)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        return TIterator{ };
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::InsertFront(const T& in)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        return Insert(Iterator(), in);
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::InsertFront(T&& in)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        return Insert(Iterator(), MOVE(in));
    }

    template <typename T, typename TAllocator>
    template <RFwdIt<T> TIt>
    constexpr auto DynamicArray<T, TAllocator>::InsertFront(TIt it)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        return Insert(Iterator(), MOVE(it));
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::InsertBack(const T& in)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        auto it = Iterator();
        it.Next(Count());
        return Insert(it, in);
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::InsertBack(T&& in)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        // auto it = Iterator();
        // it.Next(Count());
        // return Insert(it, MOVE(in));
        _vector.push_back(MOVE(in));
        return TIterator();
    }

    template <typename T, typename TAllocator>
    template <typename TIt>
    requires RFwdIt<TIt, const T>
    constexpr auto DynamicArray<T, TAllocator>::InsertBack(TIt it)
        -> typename DynamicArray<T, TAllocator>::TIterator
    {
        // auto it = Iterator();
        // it.Next(Count());
        // return Insert(it, MOVE(in));

        for (const T& e : it)
        {
            _vector.push_back(e);
        }

        return TIterator();
    }

    template <typename T, typename TAllocator>
    template <RFwdIt<T> TIt>
    constexpr auto DynamicArray<T, TAllocator>::Remove(TIt it)
        -> usize
    {
        usize count = 0;

        for (const auto& element : it)
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
    constexpr auto DynamicArray<T, TAllocator>::Remove(TIterator it)
        -> bool
    {
        auto std_it = _vector.begin() + (it.begin() - _vector.data());
        return _vector.erase(std_it) != _vector.end();
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Remove(const T& in)
        -> bool
    {
        for (auto it = Iterator(); it.HasNext(); it.Next())
        {
            if (it.Get() == in)
            {
                return Remove(it);
            }
        }

        return false;
    }

    template <typename T, typename TAllocator>
    constexpr auto DynamicArray<T, TAllocator>::Contains(const T& in) const noexcept
        -> bool
    {
        for (auto it = Iterator(); it.HasNext(); it.Next())
        {
            if (it.Get() == in)
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