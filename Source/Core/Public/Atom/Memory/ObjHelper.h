#pragma once
#include "MemHelper.h"

namespace Atom
{
    class ObjHelper
    {
    public:
        template <typename TPtr>
        constexpr auto Construct(TPtr&& mem, auto&&... args) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::construct_at(mem, fwd(args)...);
        }

        template <typename TPtr>
        constexpr auto Construct(TPtr* mem, auto&&... args) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::construct_at(mem, fwd(args)...);
        }

        template <typename TPtr>
        constexpr auto Assign(TPtr* mem, auto&& arg) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            *mem = fwd(arg);
        }

        template <typename T>
        constexpr auto Swap(T& t1, T& t2) const
        {
            T tmp = mov(t1);
            t1 = mov(t2);
            t2 = mov(tmp);
        }

        template <typename TPtr>
        constexpr auto Destruct(TPtr&& mem) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::destroy_at(mem);
        }

        template <typename TPtr>
        constexpr auto Destruct(TPtr* mem) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::destroy_at(mem);
        }
    };
}
