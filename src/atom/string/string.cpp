export module atom.core:string;
import :buf_string;
import :memory.default_mem_allocator;
import :std;
import :core;

namespace atom
{
    export class string: public buf_string<40, default_mem_allocator>
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
            const char* begin = str.to_std_char_ptr();
            atom::_usize count = str.count().unwrap();
            return hash<std::string_view>()(std::string_view(begin, count));
        }
    };
}
