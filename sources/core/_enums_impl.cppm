export module atom.core:core.enums_impl;

// import std;
// import magic_enum;
// import :core.tuple;
// import :core.option;

// namespace atom
// {
//     using string_view = std::string_view;

//     template <typename enum_type>
//     constexpr auto _enum_impl_is_flags() -> bool;

//     template <typename enum_type, bool in_is_flags>
//     class _enums_impl
//     {
//         using magic_string_type = magic_enum::string;
//         using magic_string_view_type = magic_enum::string_view;

//         template <typename value_type>
//         using magic_optional_type = magic_enum::optional<value_type>;

//         template <typename first_value_type, typename second_value_type>
//         using magic_pair_type = std::pair<first_value_type, second_value_type>;

//         template <typename value_type, usize count>
//         using magic_array_type = std::array<value_type, count>;

//     public:
//         using underlying_type = magic_enum::underlying_type_t<enum_type>;

//         static consteval auto is_flags() -> bool
//         {
//             return in_is_flags;
//         }

//         static consteval auto is_enum() -> bool
//         {
//             return typeinfo<enum_type>::is_enum;
//         }

//         static consteval auto is_scoped() -> bool
//         {
//             return magic_enum::is_scoped_enum_v<enum_type>;
//         }

//         static constexpr auto from_string(string_view str) -> option<enum_type>
//         {
//             magic_optional_type<enum_type> result = [&]
//             {
//                 if constexpr (is_flags())
//                     return magic_enum::enum_flags_cast<enum_type>(str.to_std());
//                 else
//                     magic_enum::enum_cast<enum_type>(str.to_std());
//             }();

//             if (not result.has_value())
//                 return nullopt();

//             return string_view::from_std(result.value());
//         }

//         template <typename comparer_type>
//         static constexpr auto from_string(string_view str, comparer_type&& comparer) -> option<enum_type>
//         {
//             magic_optional_type<enum_type> result = [&]
//             {
//                 if constexpr (is_flags())
//                     return magic_enum::enum_flags_cast<enum_type>(
//                         str.to_std(), forward<comparer_type>(comparer));
//                 else
//                     magic_enum::enum_cast<enum_type>(str.to_std(), forward<comparer_type>(comparer));
//             }();

//             if (not result.has_value())
//                 return nullopt();

//             return string_view::from_std(result.value());
//         }

//         static constexpr auto from_underlying_unchecked(underlying_type value) -> enum_type
//         {
//             return enum_type(value);
//         }

//         static constexpr auto from_underlying_typery(underlying_type value) -> option<enum_type>
//         {
//             magic_optional_type<enum_type> value_opt = [&]
//             {
//                 if constexpr (is_flags())
//                     return magic_enum::enum_flags_cast<enum_type>(value);
//                 else
//                     magic_enum::enum_cast<enum_type>(value);
//             }();

//             if (not value_opt.has_value())
//                 return nullopt();

//             return value_opt.value();
//         }

//         static constexpr auto from_index_unchecked(usize index) -> enum_type
//         {
//             return magic_enum::enum_value<enum_type>(index);
//         }

//         static constexpr auto from_index_typery(usize index) -> option<enum_type>
//         {
//             if (index >= get_count())
//                 return nullopt();

//             return magic_enum::enum_value<enum_type>(index);
//         }

//         static consteval auto get_type_name() -> string_view
//         {
//             return string_view::from_std(magic_enum::enum_type_name<enum_type>());
//         }

//         static consteval auto get_count() -> usize
//         {
//             return magic_enum::enum_count<enum_type>();
//         }

//         static consteval auto get_values() -> array_view<enum_type>
//         {
//             auto values = magic_enum::enum_values<enum_type>();
//             return array_view<enum_type>(values.data(), values.size());
//         }

//         static consteval auto get_strings() -> array_view<string_view>
//         {
//             magic_array_type<magic_string_view_type, get_count()> magic_strings =
//                 magic_enum::enum_names<enum_type>();

//             magic_array_type<string_view, get_count()> strings;
//             for (usize i = 0; i < get_count(); i++)
//             {
//                 strings[i] = string_view::from_std(magic_strings[i]);
//             }

//             return array_view<string_view>(strings.data(), strings.size());
//         }

//         static consteval auto get_entries() -> array_view<tuple<enum_type, string_view>>
//         {
//             magic_array_type<magic_pair_type<enum_type, magic_string_view_type>, get_count()> magic_strings =
//                 magic_enum::enum_names<enum_type>();

//             magic_array_type<tuple<enum_type, string_view>, get_count()> strings;
//             for (usize i = 0; i < get_count(); i++)
//             {
//                 strings[i] = tuple<enum_type, string_view>(
//                     magic_strings[i].first, string_view::from_std(magic_strings[i].second));
//             }

//             return array_view<tuple<enum_type, string_view>>(strings.data(), strings.size());
//         }

//         static constexpr auto is_value_valid(enum_type value) -> bool
//         {
//             if constexpr (is_flags())
//                 return magic_enum::enum_flags_contains(value);
//             else
//                 return magic_enum::enum_contains(value);
//         }

//         static constexpr auto is_index_valid(usize index) -> bool
//         {
//             return index < get_count();
//         }

//         static constexpr auto is_underlying_valid(underlying_type value) -> bool
//         {
//             if constexpr (is_flags())
//                 return magic_enum::enum_flags_contains(value);
//             else
//                 return magic_enum::enum_contains(value);
//         }

//         static constexpr auto to_index(enum_type value) -> usize
//         {
//             return magic_enum::enum_index(value).value_or(math::max<usize>());
//         }

//         static constexpr auto to_underlying(enum_type value) -> underlying_type
//         {
//             return underlying_type(value);
//         }

//         static constexpr auto to_string_view(enum_type value) -> string_view
//         {
//             return string_view::from_std(
//                 [value]
//                 {
//                     if constexpr (is_flags())
//                     {
//                         return magic_enum::enum_flags_name<enum_type>(value);
//                     }
//                     else
//                     {
//                         return magic_enum::enum_name<enum_type>(value);
//                     }
//                 }());
//         }

//         /// @todo implement this.
//         static consteval auto get_min() -> enum_type {}

//         /// @todo implement this.
//         static consteval auto get_max() -> enum_type {}

//         /// @todo implement this. this is just a workaround.
//         static consteval auto get_all_flags() -> enum_type
//         {
//             return enum_type(math::max<underlying_type>());
//         }

//         static constexpr auto add_flags(enum_type lhs, enum_type rhs) -> enum_type
//         {
//             return enum_type(underlying_type(lhs) | underlying_type(rhs));
//         }

//         static constexpr auto remove_flags(enum_type lhs, enum_type rhs) -> enum_type
//         {
//             return enum_type(underlying_type(lhs) & ~underlying_type(rhs));
//         }

//         static constexpr auto get_common_flags(enum_type lhs, enum_type rhs) -> enum_type
//         {
//             return enum_type(underlying_type(lhs) & underlying_type(rhs));
//         }

//         static constexpr auto get_uncommon_flags(enum_type lhs, enum_type rhs) -> enum_type
//         {
//             underlying_type union_flags = underlying_type(lhs) | underlying_type(rhs);
//             underlying_type intersection_flags = underlying_type(lhs) & underlying_type(rhs);
//             return enum_type(union_flags & intersection_flags);
//         }

//         static constexpr auto get_reverse_flags(enum_type flags) -> enum_type
//         {
//             return enum_type(underlying_type(get_all_flags()) & ~underlying_type(flags));
//         }

//         static constexpr auto has_all_flags(enum_type lhs, enum_type rhs) -> bool
//         {
//             return (underlying_type(lhs) & underlying_type(rhs)) == underlying_type(lhs);
//         }

//         static constexpr auto has_any_flags(enum_type lhs, enum_type rhs) -> bool
//         {
//             return (underlying_type(lhs) & underlying_type(rhs)) != 0;
//         }
//     };
// }

// template <typename enum_type>
//     requires(atom::_enums_impl<enum_type, true>::is_enum())
// struct magic_enum::customize::enum_range<enum_type>
// {
//     static constexpr bool is_flags = true;
// };
