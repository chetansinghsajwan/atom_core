#pragma once
#include "atom/exceptions.h"
#include "atom/hash/private/t1_hash.h"
#include "atom/math.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// t1_hash parser.
    /// --------------------------------------------------------------------------------------------
    template <typename t1_hash>
    class _t1_hash_parser
    {
        static constexpr usize size = t1_hash::size;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # requires
        ///
        /// - `count == (size * 2) + 1`:  size * 2 (1 byte represents 2 hex values and 1 char
        /// represents one hex value) + 1 (null terminator).
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr auto parse(const char (&str)[count]) const -> t1_hash
            requires(count == (size * 2) + 1)
        {
            return _parse_arr(str);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto parse(const range_type& range) const -> t1_hash
            requires(rrange_of<range_type, uchar>)
        {
            if constexpr (rjump_range_of<range_type, uchar>)
            {
                return _parse_jump_range(range.iter(), range.iter_end());
            }

            return _parse_range(range.iter(), range.iter_end());
        }

    private:
        template <typename iter_type, typename iter_end_type>
        constexpr auto _parse_range(iter_type it, iter_end_type it_end) const -> t1_hash
        {
            t1_hash hash;
            usize i = 0;
            while (not it.is_eq(it_end))
            {
                if (i > 20)
                {
                    return t1_hash::null;
                }

                byte hex1 = math::char_to_hex(it.value());
                if (hex1 == byte(-1))
                {
                    return t1_hash::null;
                }

                // left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;

                if (it.next().is_eq(it_end))
                {
                    return t1_hash::null;
                }

                byte hex2 = math::char_to_hex(it.value());
                if (hex2 == byte(-1))
                {
                    return t1_hash::null;
                }
                it.next();

                hash.bytes[i++] = hex1 + hex2;
            }

            if (i < 20)
            {
                return t1_hash::null;
            }

            return hash;
        }

        template <typename iter_type, typename iter_end_type>
        constexpr auto _parse_jump_range(iter_type it, iter_end_type it_end) const -> t1_hash
        {
            if (it_end.compare(it).is_eq(size * 2) == false)
            {
                return t1_hash::null;
            }

            t1_hash hash;
            for (usize i = 0; i < 20; i++)
            {
                byte hex1 = math::char_to_hex(it.value());
                if (hex1 == byte(-1))
                {
                    return t1_hash::null;
                }

                // left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;
                it.next();

                byte hex2 = math::char_to_hex(it.value());
                if (hex2 == byte(-1))
                {
                    return t1_hash::null;
                }
                it.next();

                hash.bytes[i] = hex1 + hex2;
            }

            return hash;
        }

        constexpr auto _parse_arr(mem_ptr<uchar> str) const -> t1_hash
        {
            t1_hash hash;
            for (usize i = 0; i < size * 2; i += 2)
            {
                byte hex1 = math::char_to_hex(str[i]);
                if (hex1 == byte(-1))
                {
                    return t1_hash::null;
                }

                // left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;

                byte hex2 = math::char_to_hex(str[i + 1]);
                if (hex2 == byte(-1))
                {
                    return t1_hash::null;
                }

                hash.bytes[i / 2] = hex1 + hex2;
            }

            return hash;
        }
    };
}
