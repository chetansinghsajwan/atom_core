#pragma once
#include <memory>

#include "Atom/Core.h"

namespace Atom
{
    template <typename T>
    using UniquePtr = std::unique_ptr<T>;

    template <typename T>
    auto MakeUnique(auto&&... args) -> UniquePtr<T>
    {
        return std::make_unique<T>(fwd(args)...);
    }
}