#pragma once
#include <memory>

#include "Atom/Core.h"

namespace Atom
{
    template <tname T>
    using SharedPtr = std::shared_ptr<T>;

    template <tname T>
    SharedPtr<T> MakeShared(auto&&... args) noex
    {
        return std::make_shared<T>(fwd(args)...);
    }
}