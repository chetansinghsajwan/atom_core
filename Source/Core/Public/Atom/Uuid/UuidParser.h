#pragma once
#include "Uuid.h"

#include "Atom/Range.h"
#include "Atom/Str.h"
#include "Atom/Math.h"

namespace Atom
{
    struct UuidParser
    {
        template <typename TRange>
        requires RRangeOf<TRange, Char>
        constexpr Uuid Parse(const TRange& range) const noex
        {
            RangeHelper helper;
            if constexpr (helper.CanGetCount<TRange>())
            {
                return _ParseCounted(range.Iter(), helper.GetCount(range));
            }
            else
            {
                return _ParseUncounted(range.Iter(), range.IterEnd());
            }
        }

    private:
        template <typename TIter>
        constexpr Uuid _ParseCounted(TIter it, usize itCount) const noex
        {
            if (itCount != 36)
                return Uuid::Null;

            Uuid uuid;          // output result
            usize i = 0;        // index of byte to write
            usize j = 0;        // index of char to read

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

        template <typename TIter, typename TIterEnd>
        constexpr Uuid _ParseUncounted(TIter it, TIterEnd itEnd) const noex
        {
            Uuid uuid;                          // output result
            usize i = 0;                        // index of byte to write
            usize j = 0;                        // index of char to read

            while (i < 16)
            {
                if (it == itEnd)
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
                if (it == itEnd)
                    return Uuid::Null;

                byte low = Math::CharToHex(*it);
                if (low == byte(-1))
                    return Uuid::Null;

                uuid.bytes[i++] = (high << 4) | low;

                j++; it++;
            }

            if (it != itEnd)
            {
                return Uuid::Null;
            }

            return uuid;
        }
    };
}