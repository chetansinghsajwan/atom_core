export module atom.core:strings.string;

import :core;
import :default_mem_allocator;
import :strings.buf_string;
import :strings.format_string;
import :strings.string_formatting;
import :strings.string_formatter_provider;

namespace atom
{
    export class string: public buf_string<40, default_mem_allocator>
    {
        using base_t = buf_string<40, default_mem_allocator>;

    public:
        using base_t::base_t;
        using base_t::operator=;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr auto with_capacity(usize count) -> string
        {
            string str;
            str.reserve(count);
            return move(str);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename output_t, typename... arg_ts>
        static constexpr auto format_to(
            output_t&& out, format_string<arg_ts...> fmt, arg_ts&&... args)
            requires(string_formatter_provider<arg_ts>::has() and ...)
        // requires is_output<output_t, char>
        {
            _format_to(out, fmt, atom::forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_ts>
        static constexpr auto format(format_string<arg_ts...> fmt, arg_ts&&... args) -> string
            requires(string_formatter_provider<arg_ts>::has() and ...)
        {
            string out;
            format_to(out, fmt, atom::forward<arg_ts>(args)...);

            return out;
        }
    };
}
