#pragma once
#include "MemHelper.decl.h"

namespace Atom
{
    class ObjHelper
    {
    public:
        template <typename TPtr>
        constexpr fn Construct(TPtr&& mem, auto&&... args) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::construct_at(mem, fwd(args)...);
        }

        template <typename TPtr>
        constexpr fn Construct(TPtr* mem, auto&&... args) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::construct_at(mem, fwd(args)...);
        }

        template <typename TPtr>
        constexpr fn Assign(TPtr* mem, auto&& arg) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            *mem = fwd(arg);
        }

        template <typename T>
        constexpr fn Swap(T& t1, T& t2) const
        {
            T tmp = mov(t1);
            t1 = mov(t2);
            t2 = mov(tmp);
        }

        template <typename TPtr>
        constexpr fn Destruct(TPtr&& mem) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::destroy_at(mem);
        }

        template <typename TPtr>
        constexpr fn Destruct(TPtr* mem) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::destroy_at(mem);
        }
    };
}