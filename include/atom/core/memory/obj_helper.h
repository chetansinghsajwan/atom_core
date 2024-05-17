#pragma once
#include "atom/core/core.h"
#include "atom/core/memory/mem_helper.h"
#include "atom/core/contracts.h"
// #include "atom/core/_std.h"

namespace atom
{
    class obj_helper
    {
    public:
        template <typename value_t, typename... arg_ts>
        constexpr auto construct_as(void* mem, arg_ts&&... args) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::construct_at((value_t*)mem, forward<arg_ts>(args)...);
        }

        template <typename value_t, typename... arg_ts>
        constexpr auto construct(value_t* mem, arg_ts&&... args) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::construct_at(mem, forward<arg_ts>(args)...);
        }

        template <typename value_t, typename arg_t>
        constexpr auto assign_as(value_t* mem, arg_t&& arg) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            *mem = forward<arg_t>(arg);
        }

        template <typename value_t, typename arg_t>
        constexpr auto assign(value_t* mem, arg_t&& arg) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            *mem = forward<arg_t>(arg);
        }

        template <typename value_t>
        constexpr auto swap(value_t& type1, value_t& t2) const
        {
            value_t tmp = move(type1);
            type1 = move(t2);
            t2 = move(tmp);
        }

        template <typename value_t>
        constexpr auto destruct_as(value_t* mem) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::destroy_at(mem);
        }

        template <typename value_t>
        constexpr auto destruct(value_t* mem) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            std::destroy_at(mem);
        }
    };
}
