#pragma once
#include "Atom/Uuid/Uuid.h"
#include "Atom/Str/Str.h"
#include "Atom/Math.h"

namespace Atom
{
    struct RangeHelper
    {
        template <typename TRange>
        requires RRange<TRange, typename TRange::TElem>
        constexpr bool CanGetRangeCount() const noexcept
        {
            return RFwdRange<TRange, typename TRange::TElem>;
        }

        template <typename TRange>
        requires RRange<TRange, typename TRange::TElem>
        constexpr usize GetRangeCount(const TRange& range) const noexcept
        {
            if constexpr (RJumpRange<TRange, typename TRange::TElem>)
            {
                return range.IterEnd() - range.Iter();
            }

            usize count = 0;
            for (auto& el : range) count++;
            return count;
        }
    };

    struct UuidParser
    {
        template <typename TRange>
        requires RRange<TRange, Char>
        constexpr Uuid Parse(const TRange& range) const noexcept
        {
            RangeHelper helper;
            if constexpr (helper.CanGetRangeCount<TRange>())
            {
                return _ParseCounted(range.Iter(), helper.GetRangeCount(range));
            }
            else
            {
                return _ParseUncounted(range.Iter(), range.IterEnd());
            }
        }

    private:
        template <typename TIter>
        constexpr Uuid _ParseCounted(TIter it, usize itCount) const noexcept
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
        constexpr Uuid _ParseUncounted(TIter it, TIterEnd itEnd) const noexcept
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