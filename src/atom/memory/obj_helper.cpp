export module atom.core:obj_helper;
import :core;
import :mem_helper;
import :contracts_decl;
import :ptr;
import :mem_ptr;
import :std;

namespace atom
{
    class obj_helper
    {
    public:
        template <typename type, typename... arg_types>
        constexpr auto construct_as(mut_ptr<void> mem, arg_types&&... args) const
        {
            contracts::debug_expects(mem != nullptr);

            std::construct_at(mem.as<type>().unwrap(), forward<arg_types>(args)...);
        }

        template <typename type, typename... arg_types>
        constexpr auto construct(mut_ptr<type> mem, arg_types&&... args) const
        {
            contracts::debug_expects(mem != nullptr);

            std::construct_at(mem.unwrap(), forward<arg_types>(args)...);
        }

        template <typename type, typename arg_type>
        constexpr auto assign_as(mut_ptr<type> mem, arg_type&& arg) const
        {
            contracts::debug_expects(mem != nullptr);

            mem.get_mut() = forward<arg_type>(arg);
        }

        template <typename type, typename arg_type>
        constexpr auto assign(mut_ptr<type> mem, arg_type&& arg) const
        {
            contracts::debug_expects(mem != nullptr);

            mem.get_mut() = forward<arg_type>(arg);
        }

        template <typename type>
        constexpr auto swap(type& type1, type& t2) const
        {
            type tmp = move(type1);
            type1 = move(t2);
            t2 = move(tmp);
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
