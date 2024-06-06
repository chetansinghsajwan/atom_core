export module atom.core:core.error;

import std;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export class error
    {
    public:
        constexpr error()
            : msg{}
        {}

        template <std::size_t count>
        constexpr error(const char (&msg)[count])
            : msg{ msg, count }
        {}

        template <typename string_view_type>
        constexpr error(string_view_type msg)
            : msg{ msg.get_data(), msg.get_count() }
        {}

    public:
        std::string_view msg;
    };

    export class value_exists_error: public error
    {};
}
