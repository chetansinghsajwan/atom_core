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
            return Parse(ArrayView<Char>(arr, size - 1));
        }

        template <typename TRange>
        requires RFwdRange<TRange, const Char>
        constexpr Uuid Parse(const TRange& range) const noexcept
        {
            if constexpr (RFwdJumpRange<decltype(range), Char>)
            {
                return _ParseFwdJump(MOVE(range));
            }

            Uuid uuid;
            usize i = 0;
            byte high = -1;
            for (Char ch : range)
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
        template <typename TRange>
        requires RFwdJumpRange<TRange, const Char>
        constexpr Uuid _ParseFwdJump(const TRange& range) const noexcept
        {
            if (range.Size() != 36)
            {
                return Uuid::Null;
            }

            Uuid uuid;
            usize i = 0;
            byte high = -1;
            for (Char ch : range)
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