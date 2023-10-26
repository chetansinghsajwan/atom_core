#pragma once
#include "Atom/Range/MutArrayRangeTrait.h"

namespace Atom
{
    template <typename T, usize count>
    class _StaticArrayStorage
    {
    public:
        T _arr[count];
    };

    template <typename T, usize count>
    class StaticArray:
        public _StaticArrayStorage<T, count>,
        public MutArrayRangeTrait<StaticArray<T, count>>
    {};

    template <typename T, usize count_>
    class MutArrayRangeTraitImpl<StaticArray<T, count_>>
    {
        using _Array = StaticArray<T, count_>;
        using _ArrayStorage = _StaticArrayStorage<T, count_>;

    public:
        using TElem = T;
        using TIter = ArrayIter<T>;
        using TIterEnd = TIter;
        using TMutIter = MutArrayIter<T>;
        using TMutIterEnd = TMutIter;

    public:
        constexpr auto data() const -> const T*
        {
            return _storageData();
        }

        constexpr auto mutData() -> T*
        {
            return _storageMutData();
        }

        constexpr auto count() const -> usize
        {
            return _storageCount();
        }

        constexpr auto iter() const -> TIter
        {
            return TIter{ _storageData() };
        }

        constexpr auto iterEnd() const -> TIterEnd
        {
            return TIterEnd{ _storageData() + _storageCount() };
        }

        constexpr auto mutIter() -> TMutIter
        {
            return TMutIter{ _storageMutData() };
        }

        constexpr auto mutIterEnd() -> TMutIterEnd
        {
            return TMutIterEnd{ _storageMutData() + _storageCount() };
        }

    private:
        constexpr auto _storageData() const -> const T*
        {
            return _storage()._arr;
        }

        constexpr auto _storageMutData() -> T*
        {
            return _mutStorage()._arr;
        }

        constexpr auto _storageCount() const -> usize
        {
            return count_;
        }

        constexpr auto _storage() const -> const _ArrayStorage&
        {
            return reinterpret_cast<const _ArrayStorage&>(*this);
        }

        constexpr auto _mutStorage() -> _ArrayStorage&
        {
            return reinterpret_cast<_ArrayStorage&>(*this);
        }
    };
}
