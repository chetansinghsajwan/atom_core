#pragma once
#include "MemHelper.h"

namespace Atom
{
    class ObjHelper
    {
    public:
        template <typename T, typename TPtr, typename... TArgs>
        constexpr auto ConstructAs(TPtr&& mem, TArgs&&... args) const
            requires(RPtrOf<TPtr, T>)
        {
            debug_expects(mem != nullptr);

            std::construct_at(mem.raw(), forward<TArgs>(args)...);
        }

        template <typename TPtr, typename... TArgs>
        constexpr auto Construct(TPtr&& mem, TArgs&&... args) const
            requires(RPtr<TPtr>)
        {
            debug_expects(mem != nullptr);

            std::construct_at(mem.raw(), forward<TArgs>(args)...);
        }

        template <typename T, typename TPtr, typename TArg>
        constexpr auto AssignAs(TPtr&& mem, TArg&& arg) const
            requires(RPtrOf<TPtr, T>)
        {
            debug_expects(mem != nullptr);

            *mem = forward<TArg>(arg);
        }

        template <typename TPtr, typename TArg>
        constexpr auto Assign(TPtr&& mem, TArg&& arg) const
            requires(RPtr<TPtr>)
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

        template <typename T, typename TPtr>
        constexpr auto DestructAs(TPtr&& mem) const
            requires(RPtrOf<TPtr, T>)
        {
            debug_expects(mem != nullptr);

            std::destroy_at(mem.raw());
        }


        template <typename TPtr>
        constexpr auto Destruct(TPtr&& mem) const
            requires(RPtr<TPtr>)
        {
            debug_expects(mem != nullptr);

            std::destroy_at(mem.raw());
        }
    };
}
