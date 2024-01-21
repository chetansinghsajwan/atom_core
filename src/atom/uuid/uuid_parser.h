#pragma once
#include "atom/math.h"
// #include "atom/range.h"
// #include "atom/string.h"
#include "uuid.h"

namespace atom
{
    class uuid_parser
    {
    public:
        template <typename range_type>
        constexpr auto parse(const range_type& range) const -> uuid
            requires(rrange_of<range_type, uchar>)
        {
            range_helper helper;
            if constexpr (helper.can_get_count<range_type>())
            {
                return _parse_counted(range.iter(), helper.get_count(range));
            }
            else
            {
                return _parse_uncounted(range.iter(), range.iter_end());
            }
        }

    private:
        template <typename iter_type>
        constexpr auto _parse_counted(iter_type it, usize it_count) const -> uuid
        {
            if (it_count != 36)
                return uuid::null;

            uuid uuid;   // output result
            usize i = 0; // index of byte to write
            usize j = 0; // index of char to read

            while (i < 16)
            {
                if (j == 8 || j == 13 || j == 18 || j == 23)
                {
                    if (it.value() != '-')
                    {
                        return uuid::null;
                    }

                    j++;
                    it.next();
                    continue;
                }

                byte high = math::char_to_hex(it.value());
                if (high == byte(-1))
                    return uuid::null;

                j++;
                it.next();

                byte low = math::char_to_hex(it.value());
                if (low == byte(-1))
                    return uuid::null;

                uuid.bytes[i++] = (high << 4) | low;

                j++;
                it.next();
            }

            return uuid;
        }

        template <typename iter_type, typename iter_end_type>
        constexpr auto _parse_uncounted(iter_type it, iter_end_type it_end) const -> uuid
        {
            uuid uuid;   // output result
            usize i = 0; // index of byte to write
            usize j = 0; // index of char to read

            while (i < 16)
            {
                if (it.is_eq(it_end))
                    return uuid::null;

                if (j == 8 || j == 13 || j == 18 || j == 23)
                {
                    if (it.value() != '-')
                    {
                        return uuid::null;
                    }

                    j++;
                    it.next();
                    continue;
                }

                byte high = math::char_to_hex(it.value());
                if (high == byte(-1))
                    return uuid::null;

                j++;
                it.next();
                if (it.is_eq(it_end))
                    return uuid::null;

                byte low = math::char_to_hex(it.value());
                if (low == byte(-1))
                    return uuid::null;

                uuid.bytes[i++] = (high << 4) | low;

                j++;
                it.next();
            }

            if (not it.is_eq(it_end))
            {
                return uuid::null;
            }

            return uuid;
        }
    };
}
