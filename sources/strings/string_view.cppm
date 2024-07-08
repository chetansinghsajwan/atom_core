export module atom_core:strings.string_view;

import std;
import :containers;
import :ranges;
import :strings.string_tag;

namespace atom
{
    export class string_view
        : public array_view<char>
        , public string_tag
    {
        using base_type = array_view<char>;

    public:
        explicit constexpr string_view(const char* str)
            : base_type{ ranges::from(str) }
        {}

        using base_type::base_type;
        using base_type::operator=;

    public:
        constexpr operator std::string_view() const
        {
            return { this->get_data(), this->get_count() };
        }
    };
}
