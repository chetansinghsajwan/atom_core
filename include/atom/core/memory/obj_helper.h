#pragma once
#include "atom/core/core.h"
#include "atom/core/memory/mem_helper.h"
#include "atom/core/contracts_decl.h"
#include "atom/core/_std.h"

namespace atom
{
    class obj_helper
    {
    public:
        template <typename value_type, typename... arg_types>
        constexpr auto construct_as(void* mem, arg_types&&... args) const
        {
            contracts::debug_expects(mem != nullptr);

            std::construct_at((value_type*)mem, forward<arg_types>(args)...);
        }

        template <typename value_type, typename... arg_types>
        constexpr auto construct(value_type* mem, arg_types&&... args) const
        {
            contracts::debug_expects(mem != nullptr);

            std::construct_at(mem, forward<arg_types>(args)...);
        }

        template <typename value_type, typename arg_type>
        constexpr auto assign_as(value_type* mem, arg_type&& arg) const
        {
            contracts::debug_expects(mem != nullptr);

            *mem = forward<arg_type>(arg);
        }

        template <typename value_type, typename arg_type>
        constexpr auto assign(value_type* mem, arg_type&& arg) const
        {
            contracts::debug_expects(mem != nullptr);

            *mem = forward<arg_type>(arg);
        }

        template <typename value_type>
        constexpr auto swap(value_type& type1, value_type& t2) const
        {
            value_type tmp = move(type1);
            type1 = move(t2);
            t2 = move(tmp);
        }

        template <typename value_type>
        constexpr auto destruct_as(value_type* mem) const
        {
            contracts::debug_expects(mem != nullptr);

            std::destroy_at(mem);
        }

        template <typename value_type>
        constexpr auto destruct(value_type* mem) const
        {
            contracts::debug_expects(mem != nullptr);

            std::destroy_at(mem);
        }
    };
}
