#pragma once
#include "atom/core/_std.h"
#include "atom/core/core.h"
#include "atom/core/range.h"
#include "atom/core/string/_string_type_id.h"

namespace atom
{
    using string_iter = array_iter<char>;
    using mut_string_iter = mut_array_iter<char>;
    
    template <typename final_t, typename container_t>
    class string_functions
        : public container_t
        , public _string_type_id
    {
        ATOM_STATIC_ASSERTS(is_same_as<typename container_t::value_t, char>);

    private:
        using this_t = string_functions;
        using base_t = container_t;

    public:
        using iter_t = typename base_t::iter_t;
        using iter_end_t = typename base_t::iter_end_t;
        // using mut_iter_t = typename base_t::mut_iter_t;
        // using mut_iter_end_t = typename base_t::mut_iter_end_t;

    public:
        using base_t::base_t;
        using base_t::operator=;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr string_functions(const char (&str)[count])
            : base_t(range_from(str, count))
        {}

    public:
        static constexpr auto from_std(std::string_view str)
        {
            return final_t(range_from(str));
        }

        constexpr auto to_std() const -> std::string_view
        {
            return std::string_view(base_t::get_data(), base_t::get_count());
        }
    };
}
