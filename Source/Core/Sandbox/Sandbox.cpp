#include "Atom/Core.h"
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

    ArrView<int> arr;

    arr.find(0);
    arr.at(0);
    arr[0];
    arr.front();
    arr.back();
    arr.iter();
    arr.iterEnd();
    arr.begin();
    arr.end();
    arr.canGetCount();
    // arr.compare(arr);
    // arr.contains(arr);
    arr.count();
    arr.data();
    // arr.equals(arr);
    // arr.find(arr);
    // arr.findRange(arr);

    return 0;
}