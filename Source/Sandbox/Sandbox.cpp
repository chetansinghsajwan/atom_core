#include "Atom/CoreAll.h"
#include <iostream>

using namespace Atom;

auto PrintCmdArgs(i32 argc, char** argv)
{
    for (i32 i = 1; i < argc; i++)
    {
        std::cout << argv[i.val()] << std::endl;
    }
}

class A
{
public:
    constexpr auto eq(const A& that) const -> bool
    {
        return true;
    }

    constexpr auto ne(const A& that) const -> bool
    {
        return false;
    }
};

auto main(int argc, char** argv) -> int
{
    PrintCmdArgs(argc, argv);

    A a;
    A b;

    bool v = a == b;

    return 0;
}
