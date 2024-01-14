#pragma once
#include "MemHelper.h"

namespace Atom
{
    class ObjHelper
    {
    public:
        template <typename T, typename... TArgs>
        constexpr auto ConstructAs(MutPtr<void> mem, TArgs&&... args) const
        {
            Contracts::DebugExpects(mem != nullptr);

            std::construct_at(mem.unwrap(), forward<TArgs>(args)...);
        }

        template <typename T, typename... TArgs>
        constexpr auto Construct(MutPtr<T> mem, TArgs&&... args) const
        {
            Contracts::DebugExpects(mem != nullptr);

            std::construct_at(mem.unwrap(), forward<TArgs>(args)...);
        }

        template <typename T, typename TArg>
        constexpr auto AssignAs(MutPtr<T> mem, TArg&& arg) const
        {
            Contracts::DebugExpects(mem != nullptr);

            mem.getMut() = forward<TArg>(arg);
        }

        template <typename T, typename TArg>
        constexpr auto Assign(MutPtr<T> mem, TArg&& arg) const
        {
            Contracts::DebugExpects(mem != nullptr);

            mem.getMut() = forward<TArg>(arg);
        }

        template <typename T>
        constexpr auto Swap(T& t1, T& t2) const
        {
            T tmp = mov(t1);
            t1 = mov(t2);
            t2 = mov(tmp);
        }

        template <typename T>
        constexpr auto DestructAs(MutPtr<T> mem) const
        {
            Contracts::DebugExpects(mem != nullptr);

            std::destroy_at(mem.unwrap());
        }


        template <typename T>
        constexpr auto Destruct(MutPtr<T> mem) const
        {
            Contracts::DebugExpects(mem != nullptr);

            std::destroy_at(mem.unwrap());
        }
    };
}
