#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    union UnionOf;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname T, tname... Ts>
    union UnionOf<T, Ts...>
    {
        pub cexpr ctor UnionOf() { }
        pub cexpr ctor UnionOf(const UnionOf& that) { }
        pub cexpr fn op=(const UnionOf& that) -> UnionOf& { }
        pub cexpr ctor UnionOf(UnionOf&& that) { }
        pub cexpr fn op=(UnionOf&& that) -> UnionOf& { }
        pub cexpr dtor UnionOf() { }

        pub T value;
        pub UnionOf<Ts...> next;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <>
    union UnionOf<> { };
}