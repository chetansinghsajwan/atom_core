#pragma once
#include "MemHelper.decl.h"

namespace Atom
{
    class ObjHelper
    {
        pub template <typename TPtr>
        constexpr fn Construct(TPtr&& mem, auto&&... args) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::construct_at(mem, fwd(args)...);
        }

        pub template <typename TPtr>
        constexpr fn Construct(TPtr* mem, auto&&... args) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::construct_at(mem, fwd(args)...);
        }

        pub template <typename TPtr>
        constexpr fn Assign(TPtr* mem, auto&& arg) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            *mem = fwd(arg);
        }

        pub template <typename T>
        constexpr fn Swap(T& t1, T& t2) const
        {
            T tmp = mov(t1);
            t1 = mov(t2);
            t2 = mov(tmp);
        }

        pub template <typename TPtr>
        constexpr fn Destruct(TPtr&& mem) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::destroy_at(mem);
        }

        pub template <typename TPtr>
        constexpr fn Destruct(TPtr* mem) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::destroy_at(mem);
        }
    };
}