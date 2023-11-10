#pragma once
#include "Atom/Core/Primitives.h"
#include "Atom/Core/TypeList.h"

namespace Atom
{
    template <typename... Ts>
    class AlignedUnionStorageFor
    {
    private:
        using _Types = TypeList<Ts...>;

    public:
        alignas(_Types::MaxAlign.val()) byte storage[_Types::MaxSize];
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
        constexpr auto mem(usize i = 0) const -> ConstMemPtr<void>
        {
            debug_expects(i < Size());

            return ConstMemPtr(_storage + i.val());
        }

        constexpr auto mutMem(usize i = 0) -> MemPtr<void>
        {
            debug_expects(i < Size());

            return MemPtr(_storage + i.val());
        }

        constexpr auto ref(usize i) const -> const byte&
        {
            debug_expects(i < Size());

            return _storage[i.val()];
        }

        constexpr auto mutRef(usize i) -> byte&
        {
            debug_expects(i < Size());

            return _storage[i.val()];
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
                if (_storage[i.val()] != that._storage[i.val()])
                    return false;
            }

            return true;
        }

        constexpr auto ne(const StaticStorage& that) const -> bool
        {
            return not(*this == that);
        }

    public:
        byte _storage[size.val()];
    };
}
