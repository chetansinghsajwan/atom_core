#include "Atom/CoreAll.h"
#include <iostream>

using namespace Atom;

auto PrintCmdArgs(i32 argc, char** args)
{
    for (i32 i = 1; i < argc; i++)
    {
        std::cout << args[i.unwrap()] << std::endl;
    }
}

auto main(int argc, char** args) -> int
{
    PrintCmdArgs(argc, args);

    return 0;
}
