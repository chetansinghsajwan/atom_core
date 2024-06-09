export module atom.core:strings.string;

import :core;
import :default_mem_allocator;
import :ranges;
import :strings.buf_string;
import :strings.format_string;
import :strings.string_formatting;
import :strings.string_formatter_provider;

namespace atom
{
    export class string: public buf_string<40, default_mem_allocator>
    {
        using base_type = buf_string<40, default_mem_allocator>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        constexpr string(create_from_raw_tag, const char* str)
            : base_type{ create_from_raw, str, _find_str_len(str) }
        {}

        constexpr string(create_with_join_tag, const char* str0, const char* str1)
        {
            insert_range_back(ranges::from(str0));
            insert_range_back(ranges::from(str1));
        }

        using base_type::base_type;
        using base_type::operator=;

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
        template <typename output_type, typename... arg_types>
        static constexpr auto format_to(
            output_type&& out, format_string<arg_types...> fmt, arg_types&&... args)
            requires(string_formatter_provider<arg_types>::has() and ...)
        // requires is_output<output_type, char>
        {
            _format_to(out, fmt, atom::forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        static constexpr auto format(format_string<arg_types...> fmt, arg_types&&... args) -> string
            requires(string_formatter_provider<arg_types>::has() and ...)
        {
            string out;
            format_to(out, fmt, atom::forward<arg_types>(args)...);

            return out;
        }

    private:
        /// --------------------------------------------------------------------------------------------
        /// # to do
        ///
        /// - review this implementation after implementing character encoding.
        /// --------------------------------------------------------------------------------------------
        static constexpr auto _find_str_len(const char* str) -> usize
        {
            usize len = 0;
            while (*str != '\0')
            {
                str++;
                len++;
            }

            return len;
        }
    };
}
