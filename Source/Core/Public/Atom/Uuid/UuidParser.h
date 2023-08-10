#pragma once
#include "Atom/Math.h"
#include "Atom/Range.h"
#include "Atom/Str.h"
#include "Uuid.h"

namespace Atom
{
    class UuidParser
    {
    public:
        template <typename TRange>
        constexpr auto parse(const TRange& range) const -> Uuid
            requires(RRangeOf<TRange, Char>)
        {
            RangeHelper helper;
            if constexpr (helper.CanGetCount<TRange>())
            {
                return _parseCounted(range.iter(), helper.GetCount(range));
            }
            else
            {
                return _parseUncounted(range.iter(), range.iterEnd());
            }
        }

    private:
        template <typename TIter>
        constexpr auto _parseCounted(TIter it, usize itCount) const -> Uuid
        {
            if (itCount != 36)
                return Uuid::Null;

            Uuid uuid;   // output result
            usize i = 0; // index of byte to write
            usize j = 0; // index of char to read

            while (i < 16)
            {
                if (j == 8 || j == 13 || j == 18 || j == 23)
                {
                    if (*it != '-')
                    {
                        return Uuid::Null;
                    }

                    j++;
                    it++;
                    continue;
                }

                byte high = Math::CharToHex(*it);
                if (high == byte(-1))
                    return Uuid::Null;

                j++;
                it++;

                byte low = Math::CharToHex(*it);
                if (low == byte(-1))
                    return Uuid::Null;

                uuid.bytes[i++] = (high << 4) | low;

                j++;
                it++;
            }

            return uuid;
        }

        template <typename TIter, typename TIterEnd>
        constexpr auto _parseUncounted(TIter it, TIterEnd itEnd) const -> Uuid
        {
            Uuid uuid;   // output result
            usize i = 0; // index of byte to write
            usize j = 0; // index of char to read

            while (i < 16)
            {
                if (it == itEnd)
                    return Uuid::Null;

                if (j == 8 || j == 13 || j == 18 || j == 23)
                {
                    if (*it != '-')
                    {
                        return Uuid::Null;
                    }

                    j++;
                    it++;
                    continue;
                }

                byte high = Math::CharToHex(*it);
                if (high == byte(-1))
                    return Uuid::Null;

                j++;
                it++;
                if (it == itEnd)
                    return Uuid::Null;

                byte low = Math::CharToHex(*it);
                if (low == byte(-1))
                    return Uuid::Null;

                uuid.bytes[i++] = (high << 4) | low;

                j++;
                it++;
            }

            if (it != itEnd)
            {
                return Uuid::Null;
            }

            return uuid;
        }
    };
}
