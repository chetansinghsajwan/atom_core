#pragma once
#include <memory>

#include "Atom/Core.h"

namespace Atom
{
    template <typename T>
    using UniquePtr = std::unique_ptr<T>;

    template <typename T>
    UniquePtr<T> MakeUnique(auto&&... args) noexcept
    {
        return std::make_unique<T>(FORWARD(args)...);
    }
}