#pragma once
#include "Atom/Range/MutArrRangeTrait.h"

namespace Atom
{
    template <typename T, usize count>
    class _StaticArrStorage
    {
    public:
        using TElem = T;

    public:
        constexpr auto getData() -> T*
        {
            return _arr;
        }

        constexpr auto getData() const -> const T*
        {
            return _arr;
        }

        constexpr auto getCount() const -> usize
        {
            return count;
        }

    public:
        T _arr[count];
    };

    template <typename T, usize count>
    class StaticArr:
        public _StaticArrStorage<T, count>,
        public MutArrRangeTrait<StaticArr<T, count>>
    {};

    template <typename T, usize count>
    class MutArrRangeTraitImpl<StaticArr<T, count>>
    {
    private:
        using _Arr = StaticArr<T, count>;
        using _Storage = _StaticArrStorage<T, count>;

    public:
        using TElem = T;

    public:
        constexpr auto getData() -> T*
        {
            return _storage().getData();
        }

        constexpr auto getData() const -> const T*
        {
            return _storage().getData();
        }

        constexpr auto getCount() const -> usize
        {
            return _storage().getCount();
        }

    private:
        constexpr auto _storage() -> _Storage&
        {
            return reinterpret_cast<_Storage&>(*this);
        }

        constexpr auto _storage() const -> const _Storage&
        {
            return reinterpret_cast<const _Storage&>(*this);
        }
    };
}
