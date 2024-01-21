#pragma once
#include "buf_string.h"

namespace atom
{
    class string: public buf_string<40, default_mem_allocator>
    {
        using base_type = buf_string<40, default_mem_allocator>;

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
