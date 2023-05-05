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
        constexpr Uuid Parse(const Char(&in)[size]) const noexcept
        {
            return Parse(ArrayIterator<const Char>(in, size - 1));
        }

        constexpr Uuid Parse(RInputIterator<Char> auto in) const noexcept
        {
            if constexpr (RDirectIterator<decltype(in), Char>)
            {
                return _ParseDirect(MOVE(in));
            }

            Uuid uuid;
            usize i = 0;
            byte high = -1;
            for (Char ch : in)
            {
                if (i > 36)
                {
                    return Uuid::Null;
                }

                if (i == 8 || i == 13 || i == 18 || i == 23)
                {
                    if (ch != TEXT('-'))
                    {
                        return Uuid::Null;
                    }
                }

                byte low = Math::CharToHex(ch);
                if (low == -1)
                {
                    return Uuid::Null;
                }

                if (high == -1)
                {
                    high = low;
                    continue;
                }

                uuid.bytes[i / 2] = (high << 4) | low;
                high = -1;
            }

            if (i < 36)
            {
                return Uuid::Null;
            }

            return uuid;
        }

    private:
        constexpr Uuid _ParseDirect(RDirectIterator<Char> auto in) const noexcept
        {
            if (in.Range() != 36)
            {
                return Uuid::Null;
            }

            Uuid uuid;
            usize i = 0;
            byte high = -1;
            for (Char ch : in)
            {
                if (i == 8 || i == 13 || i == 18 || i == 23)
                {
                    if (ch != TEXT('-'))
                    {
                        return Uuid::Null;
                    }
                }

                byte low = Math::CharToHex(ch);
                if (low == (byte)-1)
                {
                    return Uuid::Null;
                }

                if (high == (byte)-1)
                {
                    high = low;
                    continue;
                }

                uuid.bytes[i / 2] = (high << 4) | (low & 0b00001111);
                high = -1;
            }

            return uuid;
        }
    };
}