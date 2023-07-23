#pragma once
#include "MemHelper.decl.h"

namespace Atom
{
    class ObjHelper
    {
        pub template <tname TPtr>
        cexpr fn Construct(TPtr&& mem, auto&&... args) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::construct_at(mem, fwd(args)...);
        }

        pub template <tname TPtr>
        cexpr fn Construct(TPtr* mem, auto&&... args) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::construct_at(mem, fwd(args)...);
        }

        pub template <tname TPtr>
        cexpr fn Assign(TPtr* mem, auto&& arg) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            *mem = fwd(arg);
        }

        pub template <tname TPtr>
        cexpr fn Destruct(TPtr&& mem) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::destroy_at(mem);
        }

        pub template <tname TPtr>
        cexpr fn Destruct(TPtr* mem) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::destroy_at(mem);
        }
    };
}