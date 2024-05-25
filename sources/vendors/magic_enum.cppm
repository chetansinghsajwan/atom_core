module;
#include "magic_enum_all.hpp"

export module magic_enum;

export namespace magic_enum
{
    using magic_enum::enum_cast;
    using magic_enum::enum_contains;
    using magic_enum::enum_count;
    using magic_enum::enum_flags_cast;
    using magic_enum::enum_flags_contains;
    using magic_enum::enum_flags_name;
    using magic_enum::enum_index;
    using magic_enum::enum_name;
    using magic_enum::enum_names;
    using magic_enum::enum_type_name;
    using magic_enum::enum_value;
    using magic_enum::enum_values;
    using magic_enum::is_scoped_enum_v;
    using magic_enum::optional;
    using magic_enum::string;
    using magic_enum::string_view;
    using magic_enum::underlying_type_t;

    namespace bitwise_operators
    {
        using bitwise_operators::operator~;
        using bitwise_operators::operator|;
        using bitwise_operators::operator|=;
        using bitwise_operators::operator&;
        using bitwise_operators::operator&=;
        using bitwise_operators::operator^;
        using bitwise_operators::operator^=;
    }

    namespace customize
    {
        using customize::enum_range;
    }
}
