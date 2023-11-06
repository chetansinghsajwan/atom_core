#pragma once
#include "MemHelper.h"

namespace Atom
{
    class ObjHelper
    {
    public:
        template <typename TPtr, typename... TArgs>
        constexpr auto Construct(TPtr&& mem, TArgs&&... args) const
        {
            debug_expects(mem != nullptr);

            std::construct_at(mem, forward<TArgs>(args)...);
        }

        template <typename TPtr, typename... TArgs>
        constexpr auto Construct(TPtr* mem, TArgs&&... args) const
        {
            debug_expects(mem != nullptr);

            std::construct_at(mem, forward<TArgs>(args)...);
        }

        template <typename TPtr, typename TArg>
        constexpr auto Assign(TPtr* mem, TArg&& arg) const
        {
            debug_expects(mem != nullptr);

            *mem = forward<TArg>(arg);
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
            debug_expects(mem != nullptr);

            std::destroy_at(mem);
        }

        template <typename TPtr>
        constexpr auto Destruct(TPtr* mem) const
        {
            debug_expects(mem != nullptr);

            std::destroy_at(mem);
        }
    };
}
