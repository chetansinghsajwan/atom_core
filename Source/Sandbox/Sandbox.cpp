#include "Atom/CoreAll.h"
#include <iostream>

using namespace Atom;

auto PrintCmdArgs(i32 argc, char** argv)
{
    for (i32 i = 1; i < argc; i++)
    {
        std::cout << argv[i] << std::endl;
    }
}

auto main(int argc, char** argv) -> int
{
    PrintCmdArgs(argc, argv);

    i32 n0;
    n0.assign(i32::Max());
    n0.assign(i32::Max().val());

    return 0;
}
