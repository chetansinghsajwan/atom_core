#pragma once
#include "MemHelper.decl.h"

namespace Atom
{
    struct ObjHelper
    {
        template <typename TPtr>
        constexpr void Construct(TPtr&& mem, auto&&... args) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::construct_at(mem, FORWARD(args)...);
        }

        template <typename TPtr>
        constexpr void Destruct(TPtr&& mem) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::destroy_at(mem);
        }
    };
}