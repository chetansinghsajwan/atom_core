#pragma once
#include <memory>

#include "Atom/Core.h"

namespace Atom
{
    template <typename T>
    using UniquePtr = std::unique_ptr<T>;

    template <typename T, typename... TArgs>
    auto MakeUnique(TArgs&&... args) -> UniquePtr<T>
    {
        return std::make_unique<T>(forward<TArgs>(args)...);
    }
}
