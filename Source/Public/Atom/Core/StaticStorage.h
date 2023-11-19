#pragma once
#include "Atom/Core/Primitives.h"
#include "Atom/Core/TypeList.h"

namespace Atom
{
    template <typename... Ts>
    class StaticStorageFor
    {
    private:
        using _Types = TypeList<Ts...>;

    public:
        alignas(_Types::MaxAlign.unwrap()) byte storage[_Types::MaxSize];
    };

    template <usize size>
    class StaticStorage
    {
    public:
        static consteval auto Size() -> usize
        {
            return size;
        }

    public:
        constexpr auto mem(usize i = 0) const -> MemPtr<void>
        {
            Contracts::DebugExpects(i < Size());

            return MemPtr(_storage + i.unwrap());
        }

        constexpr auto mutMem(usize i = 0) -> MutMemPtr<void>
        {
            Contracts::DebugExpects(i < Size());

            return MutMemPtr(_storage + i.unwrap());
        }

        constexpr auto ref(usize i) const -> const byte&
        {
            Contracts::DebugExpects(i < Size());

            return _storage[i.unwrap()];
        }

        constexpr auto mutRef(usize i) -> byte&
        {
            Contracts::DebugExpects(i < Size());

            return _storage[i.unwrap()];
        }

        constexpr auto operator[](usize i) const -> const byte&
        {
            return ref(i);
        }

        constexpr auto operator[](usize i) -> byte&
        {
            return mutRef(i);
        }

        constexpr auto eq(const StaticStorage& that) const -> bool
        {
            for (usize i = usize(0); i < Size(); i++)
            {
                if (_storage[i.unwrap()] != that._storage[i.unwrap()])
                    return false;
            }

            return true;
        }

        constexpr auto ne(const StaticStorage& that) const -> bool
        {
            return not(*this == that);
        }

    public:
        byte _storage[size.unwrap()];
    };
}
