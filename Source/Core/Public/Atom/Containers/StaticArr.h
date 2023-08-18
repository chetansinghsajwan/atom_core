#pragma once
#include "Atom/Range/MutArrRangeTrait.h"

namespace Atom
{
    template <typename T, usize count>
    class _StaticArrStorage
    {
    public:
        T _arr[count];
    };

    template <typename T, usize count>
    class StaticArr:
        public _StaticArrStorage<T, count>,
        public MutArrRangeTrait<StaticArr<T, count>>
    {};

    template <typename T, usize count_>
    class MutArrRangeTraitImpl<StaticArr<T, count_>>
    {
        using _Arr = StaticArr<T, count_>;
        using _ArrStorage = _StaticArrStorage<T, count_>;

    public:
        using TElem = T;
        using TIter = ArrIter<T>;
        using TIterEnd = TIter;
        using TMutIter = MutArrIter<T>;
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

        constexpr auto _storage() const -> const _ArrStorage&
        {
            return reinterpret_cast<const _ArrStorage&>(*this);
        }

        constexpr auto _mutStorage() -> _ArrStorage&
        {
            return reinterpret_cast<_ArrStorage&>(*this);
        }
    };
}
