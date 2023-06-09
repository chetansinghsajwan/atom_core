#pragma once
#include "Atom/Uuid/Uuid.h"
#include "Atom/String/String.h"
#include "Atom/Math.h"

namespace Atom
{
    struct UuidParser
    {
        template <usize size>
        requires (size == 37)
        constexpr Uuid Parse(const Char(&arr)[size]) const noexcept
        {
            return Parse(ArrView<Char>(Range(arr, arr + size - 1)));
        }

        template <typename TRange>
        requires RRange<TRange, Char>
        constexpr Uuid Parse(const TRange& range) const noexcept
        {
            // Faster implementation.
            if constexpr (RJumpRange<TRange, Char>)
            {
                return _ParseFwdJump(range);
            }

            Uuid uuid;                  // output result
            usize i = 0;                // index of byte to write
            usize j = 0;                // index of char to read
            auto it = range.Iter();    // input range begin
            auto end = range.IterEnd();     // input range end
            while (i < 16)
            {
                if (it == end)
                    return Uuid::Null;

                if (j == 8 || j == 13 || j == 18 || j == 23)
                {
                    if (*it != TEXT('-'))
                    {
                        return Uuid::Null;
                    }

                    j++; it++;
                    continue;
                }

                byte high = Math::CharToHex(*it);
                if (high == byte(-1))
                    return Uuid::Null;

                j++; it++;
                if (it == end)
                    return Uuid::Null;

                byte low = Math::CharToHex(*it);
                if (low == byte(-1))
                    return Uuid::Null;

                uuid.bytes[i++] = (high << 4) | low;

                j++; it++;
            }

            if (it != end)
            {
                return Uuid::Null;
            }

            return uuid;
        }

    private:
        template <typename TRange>
        requires RJumpRange<TRange, Char>
        constexpr Uuid _ParseFwdJump(const TRange& range) const noexcept
        {
            Uuid uuid;                  // output result
            usize i = 0;                // index of byte to write
            usize j = 0;                // index of char to read
            auto it = range.Iter();    // input range begin
            auto end = range.IterEnd();     // input range end

            if (it - end != 36)
                return Uuid::Null;

            while (i < 16)
            {
                if (j == 8 || j == 13 || j == 18 || j == 23)
                {
                    if (*it != TEXT('-'))
                    {
                        return Uuid::Null;
                    }

                    j++; it++;
                    continue;
                }

                byte high = Math::CharToHex(*it);
                if (high == byte(-1))
                    return Uuid::Null;

                j++; it++;

                byte low = Math::CharToHex(*it);
                if (low == byte(-1))
                    return Uuid::Null;

                uuid.bytes[i++] = (high << 4) | low;

                j++; it++;
            }

            return uuid;
        }
    };
}