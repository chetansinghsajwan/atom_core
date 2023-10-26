#pragma once
#include "Atom/Math.h"
#include "Atom/Range.h"
#include "Atom/String.h"
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
                    if (it.value() != '-')
                    {
                        return Uuid::Null;
                    }

                    j++;
                    it.next();
                    continue;
                }

                byte high = Math::CharToHex(it.value());
                if (high == byte(-1))
                    return Uuid::Null;

                j++;
                it.next();

                byte low = Math::CharToHex(it.value());
                if (low == byte(-1))
                    return Uuid::Null;

                uuid.bytes[i++] = (high << 4) | low;

                j++;
                it.next();
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
                if (it.equals(itEnd))
                    return Uuid::Null;

                if (j == 8 || j == 13 || j == 18 || j == 23)
                {
                    if (it.value() != '-')
                    {
                        return Uuid::Null;
                    }

                    j++;
                    it.next();
                    continue;
                }

                byte high = Math::CharToHex(it.value());
                if (high == byte(-1))
                    return Uuid::Null;

                j++;
                it.next();
                if (it.equals(itEnd))
                    return Uuid::Null;

                byte low = Math::CharToHex(it.value());
                if (low == byte(-1))
                    return Uuid::Null;

                uuid.bytes[i++] = (high << 4) | low;

                j++;
                it.next();
            }

            if (not it.equals(itEnd))
            {
                return Uuid::Null;
            }

            return uuid;
        }
    };
}
