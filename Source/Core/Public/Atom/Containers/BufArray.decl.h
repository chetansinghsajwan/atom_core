#pragma once
#include "DynamicArray.decl.h"

namespace Atom
{
    template <typename T, usize BufSize, typename TMemAlloc = DefaultMemAllocator>
    class BufArray: public DynamicArray<T, TMemAlloc>
    {
    public:
        using DynamicArray<T, TMemAlloc>::DynamicArray;
        using DynamicArray<T, TMemAlloc>::operator =;
    };
}