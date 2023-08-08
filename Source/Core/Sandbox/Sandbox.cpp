#include "Atom/Core.h"
#include "Atom/Range.h"
#include "Atom/Containers/ArrView.h"

using namespace Atom;

fn PrintCmdArgs(i32 argc, char** argv)
{
    for (i32 i = 1; i < argc; ++i)
    {
        std::cout << argv[i] << std::endl;
    }
}

fn main(i32 argc, char **argv) -> i32
{
    PrintCmdArgs(argc, argv);

    ArrView<int> view;

    view.find(0);

    return 0;
}