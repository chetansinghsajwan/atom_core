export module atom_core:strings.string_extensions;

import std;
import :core;
import :ranges;
import :strings._string_type_id;

namespace atom
{
    export using string_iterator = array_iterator<char>;
    export using mut_string_iterator = mut_array_iterator<char>;

    export template <typename final_type>
    class string_extensions: public _string_type_id
    {
    private:
        using this_type = string_extensions;

    public:
        static constexpr auto from_empty()
        {
            return final_type();
        }

        static constexpr auto from_std(std::string_view str)
        {
            return final_type(ranges::from(str));
        }

        static constexpr auto from_cstr(const char* str)
        {
            return final_type(ranges::from(str));
        }

        static constexpr auto from_cstr(const char* str, const usize max_count)
        {
            usize count = max_count;
            for (usize i = 0; i < max_count; i++)
            {
                if (str[i] == '\0')
                {
                    count = i + 1;
                    break;
                }
            }

            return final_type(ranges::from(str, count));
        }

        template <typename this_string_type>
        constexpr auto to_std(this const this_string_type& self) -> std::string_view
        {
            return std::string_view(self.get_data(), self.get_count());
        }
    };
}
