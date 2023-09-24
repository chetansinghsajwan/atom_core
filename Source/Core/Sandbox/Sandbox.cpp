#include "Atom/Core.h"
#include "Atom/Core/TypeList.h"

using namespace Atom;

auto PrintCmdArgs(i32 argc, char** argv)
{
    for (i32 i = 1; i < argc; i++)
    {
        std::cout << argv[i] << std::endl;
    }
}

template <usize i>
class Type
{
public:
    static constexpr usize Value = i + 1;
};

auto main(int argc, char** argv) -> int
{
    PrintCmdArgs(argc, argv);

    i32 n0;
    bool result = n0 > 0;

    static_assert(RDefaultConstructible<i32>);
    static_assert(RCopyConstructible<i32>);
    static_assert(RCopyAssignable<i32>);
    static_assert(RMoveConstructible<i32>);
    static_assert(RMoveAssignable<i32>);
    static_assert(RDestructible<i32>);

    static_assert(RTriviallyDefaultConstructible<i32>);
    static_assert(RTriviallyCopyConstructible<i32>);
    static_assert(RTriviallyCopyAssignable<i32>);
    static_assert(RTriviallyMoveConstructible<i32>);
    static_assert(RTriviallyMoveAssignable<i32>);
    static_assert(RTriviallyDestructible<i32>);

    return 0;
}
