#pragma once

// #include <typeinfo>
// #include <string_view>

/// ------------------------------------------------------------------------------------------------
/// # to do
///
/// - review these types, maybe we should remove them.
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    using nullptr_t = std::nullptr_t;
    // static constexpr nullptr_t nullptr;

    using null_val = nullptr_t;
    constexpr null_val nullval = null_val();

    using null_pos = nullptr_t;
    constexpr null_pos nullpos = null_pos();

    using null_type = nullptr_t;
    using type_info = std::type_info;

    using std_string_view = std::string_view;

    template <typename... arg_types>
    class type_holder
    {};
}
