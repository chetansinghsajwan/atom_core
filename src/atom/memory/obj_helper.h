#pragma once
#include "mem_helper.h"

namespace atom
{
    class obj_helper
    {
    public:
        template <typename type, typename... args_type>
        constexpr auto construct_as(mut_ptr<void> mem, args_type&&... args) const
        {
            contracts::debug_expects(mem != nullptr);

            std::construct_at(mem.as<type>().unwrap(), forward<args_type>(args)...);
        }

        template <typename type, typename... args_type>
        constexpr auto construct(mut_ptr<type> mem, args_type&&... args) const
        {
            contracts::debug_expects(mem != nullptr);

            std::construct_at(mem.unwrap(), forward<args_type>(args)...);
        }

        template <typename type, typename targ>
        constexpr auto assign_as(mut_ptr<type> mem, targ&& arg) const
        {
            contracts::debug_expects(mem != nullptr);

            mem.get_mut() = forward<targ>(arg);
        }

        template <typename type, typename targ>
        constexpr auto assign(mut_ptr<type> mem, targ&& arg) const
        {
            contracts::debug_expects(mem != nullptr);

            mem.get_mut() = forward<targ>(arg);
        }

        template <typename type>
        constexpr auto swap(type& t1, type& t2) const
        {
            type tmp = mov(t1);
            t1 = mov(t2);
            t2 = mov(tmp);
        }

        template <typename type>
        constexpr auto destruct_as(mut_ptr<type> mem) const
        {
            contracts::debug_expects(mem != nullptr);

            std::destroy_at(mem.unwrap());
        }

        template <typename type>
        constexpr auto destruct(mut_ptr<type> mem) const
        {
            contracts::debug_expects(mem != nullptr);

            std::destroy_at(mem.unwrap());
        }
    };
}
