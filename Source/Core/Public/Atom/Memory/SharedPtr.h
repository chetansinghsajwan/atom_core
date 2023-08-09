#pragma once
#include <memory>

#include "Atom/Core.h"

namespace Atom
{
    template <typename T>
    using SharedPtr = std::shared_ptr<T>;

    template <typename T>
    fn MakeShared(auto&&... args) noex -> SharedPtr<T>
    {
        return std::make_shared<T>(fwd(args)...);
    }
}