#pragma once
#include "basic_string.h"
#include "default_encoding.h"

namespace atom
{
    class string: public basic_string<char_encoding>
    {
        using base_type = basic_string<char_encoding>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}

namespace std
{
    template <>
    class hash<atom::string>
    {
    public:
        auto operator()(const atom::string& str) const -> std::size_t
        {
            using std_stirng_view = basic_string_view<char>;

            const char* begin = str.to_std_char_ptr();
            atom::_usize count = str.count().unwrap();
            return hash<std_stirng_view>()(std_stirng_view(begin, count));
        }
    };
}
