#pragma once
#include "MemHelper.decl.h"
#include "Atom/Range.h"

namespace Atom
{
    struct ObjHelper
    {
        template <typename TPtr>
        constexpr void Construct(TPtr&& mem, auto&&... args) const
        {
            std::construct_at(mem, FORWARD(args)...);
        }

        template <typename TPtr>
        constexpr void Destruct(TPtr&& obj) const
        {
            std::destroy_at(obj);
        }
    };
}