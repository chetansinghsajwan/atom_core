// #include "Atom/CoreAll.h"
#include "Atom/Memory/MemPtr.h"
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

    {
        ConstPtr<void> cvoidptr;
        ConstPtr<int> cintptr;

        Ptr<void> voidptr;
        Ptr<int> intptr;

        ConstPtr<int> cintptr0 = (const int*)nullptr;
        ConstPtr<int> cintptr1 = (int*)nullptr;
        ConstPtr<int> cintptr2 = cvoidptr;
        ConstPtr<int> cintptr3 = voidptr;

        cintptr0 = (const int*)nullptr;
        cintptr1 = (int*)nullptr;
        cintptr2 = cvoidptr;
        cintptr3 = voidptr;

        Ptr<int> intptr0 = (int*)nullptr;
        Ptr<int> intptr1 = voidptr;

        intptr0 = (int*)nullptr;
        intptr1 = voidptr;

        ConstPtr<void> cvoidptr0 = cintptr;
        ConstPtr<void> cvoidptr1 = intptr;

        cvoidptr0 = cintptr;
        cvoidptr1 = intptr;

        Ptr<void> voidptr0 = intptr;

        voidptr0 = intptr;
    }

    {
        ConstPtr<void> constPtrVoid;
        ConstPtr<int> constPtrInt;
        Ptr<void> ptrVoid;
        Ptr<int> ptrInt;

        ConstMemPtr<void> constMemPtrVoid;
        ConstMemPtr<int> constMemPtrInt;
        MemPtr<void> memPtrVoid;
        MemPtr<int> memPtrInt;

        ConstPtr<void> constPtrVoid0 = constMemPtrVoid;
        ConstPtr<int> constPtrInt0 = constMemPtrInt;
        Ptr<void> ptrVoid0 = memPtrVoid;
        Ptr<int> ptrInt0 = memPtrInt;
    }

    {
        ConstMemPtr<void> cvoidptr;
        ConstMemPtr<int> cintptr;

        MemPtr<void> voidptr;
        MemPtr<int> intptr;

        ConstMemPtr<int> cintptr0 = (const int*)nullptr;
        ConstMemPtr<int> cintptr1 = (int*)nullptr;
        ConstMemPtr<int> cintptr2 = cvoidptr;
        ConstMemPtr<int> cintptr3 = voidptr;

        cintptr0 = (const int*)nullptr;
        cintptr1 = (int*)nullptr;
        cintptr2 = cvoidptr;
        cintptr3 = voidptr;

        MemPtr<int> intptr1 = (int*)nullptr;
        MemPtr<int> intptr3 = voidptr;

        intptr1 = (int*)nullptr;
        intptr3 = voidptr;

        ConstMemPtr<void> cvoidptr0 = cintptr;
        ConstMemPtr<void> cvoidptr1 = intptr;

        cvoidptr0 = cintptr;
        cvoidptr1 = intptr;
    }

    {
        ConstMemPtr<int> constmemptrint = ConstMemPtr<void>();
        ConstMemPtr<void> constmemptrvoid = ConstMemPtr<int>();

        MemPtr<int> memptrint = MemPtr<void>();
        MemPtr<void> memptrvoid = MemPtr<int>();
    }

    return 0;
}
