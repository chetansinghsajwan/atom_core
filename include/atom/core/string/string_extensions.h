#pragma once
#include "atom/core/_std.h"
#include "atom/core/core.h"
#include "atom/core/range.h"
#include "atom/core/string/_string_type_id.h"

namespace atom
{
    using string_iter = array_iter<char>;
    using mut_string_iter = mut_array_iter<char>;

    template <typename final_t>
    class string_extensions: public _string_type_id
    {
    private:
        using this_t = string_extensions;

    public:
        static constexpr auto from_std(std::string_view str)
        {
            return final_t(range_from(str));
        }

        template <typename this_string_type>
        constexpr auto to_std(this const this_string_type& self) -> std::string_view
        {
            return std::string_view(self.get_data(), self.get_count());
        }
    };
}
