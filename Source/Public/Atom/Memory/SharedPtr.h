#pragma once
#include <memory>

#include "Atom/Core.h"

namespace Atom
{
    template <typename T>
    using SharedPtr = std::shared_ptr<T>;

    template <typename T, typename... TArgs>
    auto MakeShared(TArgs&&... args) -> SharedPtr<T>
    {
        return std::make_shared<T>(forward<TArgs>(args)...);
    }
}
