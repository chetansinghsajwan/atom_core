#include <iostream>
#include "Atom/Containers/DynamicArray.h"

using namespace Atom;

int main()
{
    DynamicArray<int> arr;
    std::cout << arr.Count() << std::endl;

    return 0;
}