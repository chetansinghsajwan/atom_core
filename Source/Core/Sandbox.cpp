#include "Atom/Core.h"
#include "Atom/Core/Variant.h"

using namespace Atom;

void PrintCmdArgs(i32 argc, char** argv)
{
    for (i32 i = 1; i < argc; ++i)
    {
        std::cout << argv[i] << std::endl;
    }
}

i32 main(i32 argc, char **argv)
{
    PrintCmdArgs(argc, argv);

    return 0;
}