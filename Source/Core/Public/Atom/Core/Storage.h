#include "TypeList.h"

namespace Atom
{
    template <typename... Ts>
    class AlignedUnionStorageFor
    {
        priv using _Types = TypeList<Ts...>;

        pub alignas(_Types::MaxAlign) byte storage[_Types::MaxSize];
    };
}