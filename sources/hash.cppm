export module atom_core:hash;

import std;
import :strings;

namespace std
{
    export template <>
    struct hash<atom::string>
    {
        constexpr auto operator()(const atom::string& str) const -> std::size_t
        {
            return hash<std::string_view>()(str.to_std());
        }
    };

    export template <>
    struct hash<atom::string_view>
    {
        constexpr auto operator()(const atom::string_view& str) const -> std::size_t
        {
            return hash<std::string_view>()(str.to_std());
        }
    };
}
