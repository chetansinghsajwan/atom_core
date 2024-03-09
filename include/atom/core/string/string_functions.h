#pragma once
#include "atom/core/_std.h"
#include "atom/core/core.h"
#include "atom/core/range.h"
#include "atom/core/string/_string_type_id.h"

namespace atom
{
    template <typename final_type, typename container_type>
    class string_functions
        : public container_type
        , public _string_type_id
    {
        ATOM_STATIC_ASSERTS(rsame_as<typename container_type::elem_type, char>);

    private:
        using this_type = string_functions;
        using base_type = container_type;

    public:
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;
        // using mut_iter_type = typename base_type::mut_iter_type;
        // using mut_iter_end_type = typename base_type::mut_iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr string_functions(const char (&str)[count])
            : base_type(range_from(str, count))
        {}

    public:
        static constexpr auto from_std(std::string_view str)
        {
            return final_type(range_from(str));
        }

        constexpr auto to_std() const -> std::string_view
        {
            return std::string_view(base_type::get_data(), base_type::get_count());
        }
    };
}
