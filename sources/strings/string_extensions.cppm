export module atom.core:strings.string_extensions;

import std;
import :core;
import :ranges;
import :strings._string_type_id;

namespace atom
{
    export using string_iter = array_iter<char>;
    export using mut_string_iter = mut_array_iter<char>;

    export template <typename final_t>
    class string_extensions: public _string_type_id
    {
    private:
        using this_t = string_extensions;

    public:
        static constexpr auto from_empty()
        {
            return final_t();
        }

        static constexpr auto from_std(std::string_view str)
        {
            return final_t(range_from(str));
        }

        static constexpr auto from_cstr(const char* str)
        {
            return final_t(range_from(str));
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

            return final_t(range_from(str, count));
        }

        template <typename this_string_type>
        constexpr auto to_std(this const this_string_type& self) -> std::string_view
        {
            return std::string_view(self.get_data(), self.get_count());
        }
    };
}
