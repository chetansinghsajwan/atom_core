#pragma once
#include "atom/range.h"
#include "atom/text/basic_char.h"
#include "atom/text/basic_rune.h"

namespace atom
{
    template <typename in_encoding_type, typename container_type>
    class _string_impl: public container_type
    {
    public:
        using encoding_type = in_encoding_type;
        using char_type = basic_char<encoding_type>;
        using iter_type = typename container_type::iter_type;
        using iter_end_type = typename container_type::iter_end_type;
        // using mut_iter_type             = typename container_type::mut_iter_type;
        // using mut_iter_end_type          = typename container_type::mut_iter_end_type;

        using rune_type = basic_rune<encoding_type>;
        // using rune_iter_type            = typename container_type::rune_iter_type;
        // using rune_iter_end_type         = typename container_type::rune_iter_end_type;
        // using mut_rune_iter_type         = typename container_type::mut_rune_iter_type;
        // using mut_rune_iter_end_type      = typename container_type::mut_rune_iter_end_type;

    public:
        using container_type::container_type;
        using container_type::operator=;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr _string_impl(const char_type (&arr)[count])
            : container_type()
        {}

    public:
        constexpr auto to_std_char_ptr() const -> const char*
        {
            return _to_std_char_ptr(this->data().unwrap());
        }

        constexpr auto to_std_char_ptr() -> char*
        {
            return _to_std_char_ptr(this->mut_data().unwrap());
        }
    };
}
