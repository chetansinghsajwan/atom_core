#include "Atom/Core.h"

using namespace Atom;

void PrintCmdArgs(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
    {
        std::cout << argv[i] << std::endl;
    }
}

int main(int argc, char **argv)
{
    PrintCmdArgs(argc, argv);
    
    return 0;
}