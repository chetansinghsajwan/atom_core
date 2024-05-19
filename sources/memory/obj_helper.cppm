export module atom.core:obj_helper;

import std;
import :mem_helper;
import :contracts;

namespace atom
{
    export class obj_helper
    {
    public:
        template <typename value_t, typename... arg_ts>
        constexpr auto construct_as(void* mem, arg_ts&&... args) const
        {
            contract_debug_expects(mem != nullptr);

            std::construct_at((value_t*)mem, std::forward<arg_ts>(args)...);
        }

        template <typename value_t, typename... arg_ts>
        constexpr auto construct(value_t* mem, arg_ts&&... args) const
        {
            contract_debug_expects(mem != nullptr);

            std::construct_at(mem, std::forward<arg_ts>(args)...);
        }

        template <typename value_t, typename arg_t>
        constexpr auto assign_as(value_t* mem, arg_t&& arg) const
        {
            contract_debug_expects(mem != nullptr);

            *mem = std::forward<arg_t>(arg);
        }

        template <typename value_t, typename arg_t>
        constexpr auto assign(value_t* mem, arg_t&& arg) const
        {
            contract_debug_expects(mem != nullptr);

            *mem = std::forward<arg_t>(arg);
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
            contract_debug_expects(mem != nullptr);

            std::destroy_at(mem);
        }

        template <typename value_t>
        constexpr auto destruct(value_t* mem) const
        {
            contract_debug_expects(mem != nullptr);

            std::destroy_at(mem);
        }
    };
}
