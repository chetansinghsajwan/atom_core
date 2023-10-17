#include "Atom/Core.h"
#include "Atom/Str/StackStr.h"

#include <string>

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

    i32 num;
    auto str = num.toStr();

    return 0;
}
