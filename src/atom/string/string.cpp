export module atom.core:string;
import :std;
import :fmt;
import :core;
import :buf_string;
import :string_formatting;
import :output_requirements;
import :memory.default_mem_allocator;

namespace atom
{
    export class string: public buf_string<40, default_mem_allocator>
    {
        using base_type = buf_string<40, default_mem_allocator>;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename output_type, typename... arg_types>
        static constexpr auto format_to(
            output_type&& out, format_string<arg_types...> fmt, arg_types&&... args)
            requires routput<output_type, uchar>
        {
            class out_iter_wrap
            {
            public:
                auto operator++(int) -> out_iter_wrap&
                {
                    return *this;
                }

                auto operator*() -> out_iter_wrap&
                {
                    return *this;
                }

                auto operator=(char ch) -> out_iter_wrap&
                {
                    *out += uchar(ch);
                    return *this;
                }

            public:
                output_type* out;
            };

            try
            {
                fmt::format_to(out_iter_wrap(&out), forward<arg_types>(args)...);
            }
            catch (const fmt::format_error& err)
            {
                throw _fmt_error_to_string_format_error(err);
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        static constexpr auto format(format_string<arg_types...> fmt, arg_types&&... args) -> string
        {
            string out;
            format_to(out, fmt, forward<arg_types>(args)...);

            return out;
        }
    };
}

namespace std
{
    template <>
    class hash<atom::string>
    {
    public:
        constexpr auto operator()(const atom::string& str) const -> std::size_t
        {
            const char* begin = str.to_std_char_ptr();
            atom::_usize count = str.count().unwrap();
            return hash<std::string_view>()(std::string_view(begin, count));
        }
    };
}
