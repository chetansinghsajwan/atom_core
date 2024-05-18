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
            : msg()
        {}

        template <typename string_view_t>
        constexpr error(string_view_t msg)
            : msg(msg.get_data(), msg.get_count())
        {}

    public:
        std::string_view msg;
    };

    export class value_exists_error: public error
    {};
}
