// #include "Atom/CoreAll.h"
#include "Atom/Contracts.h"
#include "Atom/Range/Range.h"
#include <iostream>

using namespace Atom;

auto PrintCmdArgs(i32 argc, char** argv)
{
    for (i32 i = 1; i < argc; i++)
    {
        std::cout << argv[i.val()] << std::endl;
    }
}

auto main(int argc, char** argv) -> int
{
    PrintCmdArgs(argc, argv);

    static i32 arr[] = { 0, 1, 2, 3 };
    const char* str = "hello";

    auto range0 = MakeRange(arr, arr + 3 );
    auto range1 = MakeRange(arr, 3 );
    auto range2 = MakeRange({ 0, 1, 2, 3 });
    auto range3 = MakeRange(arr);
    auto range4 = MakeRange(str);

    auto it = range0.iter();

    return 0;
}
