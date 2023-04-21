#pragma once
#include "Atom/Containers/ArrayIterator.h"
#include "DynamicArray.decl.h"

namespace Atom
{
    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray() noexcept:
        _vector{ } { }

    template <typename T, typename TAllocator>
    template <RInputIterator<T> TInput>
    constexpr DynamicArray<T, TAllocator>::DynamicArray(TInput in)
    {
        InsertBack(MOVE(in));
    }

    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray(const STD_TVector& vec) noexcept:
        _vector{ vec } { }

    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::DynamicArray(STD_TVector&& vec) noexcept:
        _vector{ MOVE(vec) } { }

    template <typename T, typename TAllocator>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        operator += (const T& in)
    {
        return InsertBack(in);
    }

    template <typename T, typename TAllocator>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        operator += (T&& in)
    {
        return InsertBack(MOVE(in));
    }

    template <typename T, typename TAllocator>
    template <RInputIterator<T> TInput>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        operator += (TInput in)
    {
        return InsertBack(MOVE(in));
    }

    template <typename T, typename TAllocator>
    constexpr DynamicArray<T, TAllocator>::~DynamicArray() { }

    template <typename T, typename TAllocator>
    constexpr T* DynamicArray<T, TAllocator>::Data() noexcept
    {
        return _vector.data();
    }

    template <typename T, typename TAllocator>
    constexpr const T* DynamicArray<T, TAllocator>::Data() const noexcept
    {
        return _vector.data();
    }

    template <typename T, typename TAllocator>
    constexpr usize DynamicArray<T, TAllocator>::Count() const noexcept
    {
        return _vector.size();
    }

    template <typename T, typename TAllocator>
    constexpr bool DynamicArray<T, TAllocator>::IsEmpty() const noexcept
    {
        return Count() == 0;
    }

    template <typename T, typename TAllocator>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        Iterator() noexcept
    {
        return TIterator(_vector.data(), _vector.size());
    }

    template <typename T, typename TAllocator>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        Insert(TIterator pos, const T& in)
    {
        usize index = _vector.data() - pos.begin();
        auto out = _vector.insert(_vector.begin() + index, in);
        return TIterator{ _vector.data() + index, Count() - index };
    }

    template <typename T, typename TAllocator>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        Insert(TIterator pos, T&& in)
    {
        usize index = _vector.data() - pos.begin();
        auto out = _vector.insert(_vector.begin() + index, MOVE(in));
        return TIterator{ _vector.data() + index, Count() - index };
    }

    template <typename T, typename TAllocator>
    template <RInputIterator<T> TInput>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        Insert(TIterator pos, TInput in)
    {
        return TIterator{ };
    }

    template <typename T, typename TAllocator>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        InsertFront(const T& in)
    {
        return Insert(Iterator(), in);
    }

    template <typename T, typename TAllocator>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        InsertFront(T&& in)
    {
        return Insert(Iterator(), MOVE(in));
    }

    template <typename T, typename TAllocator>
    template <RInputIterator<T> TInput>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        InsertFront(TInput in)
    {
        return Insert(Iterator(), MOVE(in));
    }

    template <typename T, typename TAllocator>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        InsertBack(const T& in)
    {
        auto it = Iterator();
        it.Next(Count());
        return Insert(it, in);
    }

    template <typename T, typename TAllocator>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        InsertBack(T&& in)
    {
        auto it = Iterator();
        it.Next(Count());
        return Insert(it, MOVE(in));
    }

    template <typename T, typename TAllocator>
    template <RInputIterator<T> TInput>
    constexpr typename DynamicArray<T, TAllocator>::TIterator DynamicArray<T, TAllocator>::
        InsertBack(TInput in)
    {
        auto it = Iterator();
        it.Next(Count());
        return Insert(it, MOVE(in));
    }

    template <typename T, typename TAllocator>
    template <RInputIterator<T> TInput>
    constexpr usize DynamicArray<T, TAllocator>::Remove(TInput in)
    {
        usize count = 0;

        for (const auto& element : in)
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
    template <RInvocable<bool, const T&> TFilter>
    constexpr usize DynamicArray<T, TAllocator>::Remove(TFilter&& filter)
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
}