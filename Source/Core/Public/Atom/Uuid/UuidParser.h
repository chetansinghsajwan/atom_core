#pragma once
#include "Uuid.h"
#include "Atom/Range.h"
#include "Atom/Str.h"
#include "Atom/Math.h"

namespace Atom
{
    class UuidParser
    {
        pub template <tname TRange>
        requires RRangeOf<TRange, Char>
        cexpr fn Parse(const TRange& range) const noex -> Uuid
        {
            RangeHelper helper;
            if cexpr (helper.CanGetCount<TRange>())
            {
                return _ParseCounted(range.Iter(), helper.GetCount(range));
            }
            else
            {
                return _ParseUncounted(range.Iter(), range.IterEnd());
            }
        }

        priv template <tname TIter>
        cexpr fn _ParseCounted(TIter it, usize itCount) const noex -> Uuid
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
                    if (*it != '-')
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

        priv template <tname TIter, tname TIterEnd>
        cexpr fn _ParseUncounted(TIter it, TIterEnd itEnd) const noex -> Uuid
        {
            Uuid uuid;         // output result
            usize i = 0;       // index of byte to write
            usize j = 0;       // index of char to read

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