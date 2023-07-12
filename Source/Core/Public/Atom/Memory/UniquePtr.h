#pragma once
#include <memory>

#include "Atom/Core.h"

namespace Atom
{
    template <tname T>
    using UniquePtr = std::unique_ptr<T>;

    template <tname T>
    UniquePtr<T> MakeUnique(auto&&... args) noex
    {
        return std::make_unique<T>(FORWARD(args)...);
    }
}