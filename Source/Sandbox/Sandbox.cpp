#include "Atom/CoreAll.h"
import atom.core;
using namespace Atom;

auto print_cmd_args(i32 argc, char** args)
{
    for (i32 i = 1; i < argc; i++)
    {
        std::cout << args[i.unwrap()] << std::endl;
    }
}

auto main(int argc, char** args) -> int
{
    print_cmd_args(argc, args);

    return 0;
}
