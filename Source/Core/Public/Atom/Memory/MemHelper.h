#pragma once
#include "Atom/Core.h"

namespace Atom
{
    struct MemHelper
    {
        constexpr void Copy(const void* src, usize size, void* dest) const noexcept
        {
        }

        constexpr void CopyFwd(const void* src, usize size, usize steps) const noexcept
        {
        }
    };
}