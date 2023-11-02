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

    Box<i32> box1 = i32(0);
    CopyBox<i32> box2 = i32(0);
    MoveBox<i32> box3 = i32(0);
    CopyMoveBox<i32> box4 = i32(0);

    return 0;
}
