#pragma once
#include <memory>

#include "Atom/Core.h"

namespace Atom
{
    template <typename T>
    using SharedPtr = std::shared_ptr<T>;

    template <typename T>
    SharedPtr<T> MakeShared(auto&&... args) noexcept
    {
        return std::make_shared<T>(FWD(args)...);
    }
}