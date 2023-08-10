#include "TypeList.h"

namespace Atom
{
    template <typename... Ts>
    class AlignedUnionStorageFor
    {
    private:
        using _Types = TypeList<Ts...>;

    public:
        alignas(_Types::MaxAlign) byte storage[_Types::MaxSize];
    };
}
