#pragma once
#include "Atom/Exceptions.h"
#include "Atom/Hash/Private/T1Hash.h"
#include "Atom/Math.h"

namespace Atom::Private
{
    /// --------------------------------------------------------------------------------------------
    /// T1Hash Parser.
    /// --------------------------------------------------------------------------------------------
    template <typename T1Hash>
    class T1HashParser
    {
        static constexpr usize Size = T1Hash::Size;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Requires
        ///
        /// - `count == (Size * 2) + 1`:  Size * 2 (1 byte represents 2 hex values and 1 char
        /// represents one hex value) + 1 (null terminator).
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr auto parse(const Char (&str)[count]) const -> T1Hash
            requires(count == (Size * 2) + 1)
        {
            return _parseArr(str);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr auto parse(const TRange& range) const -> T1Hash
            requires(RRangeOf<TRange, Char>)
        {
            if constexpr (RJumpRangeOf<TRange, Char>)
            {
                return _parseJumpRange(range.iter(), range.iterEnd());
            }

            return _parseRange(range.iter(), range.iterEnd());
        }

    private:
        template <typename TIter, typename TIterEnd>
        constexpr auto _parseRange(TIter it, TIterEnd itEnd) const -> T1Hash
        {
            T1Hash hash;
            usize i = 0;
            while (not it.equals(itEnd))
            {
                if (i > 20)
                {
                    return T1Hash::Null;
                }

                byte hex1 = Math::CharToHex(it.value());
                if (hex1 == byte(-1))
                {
                    return T1Hash::Null;
                }

                // Left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;

                if (it.next().equals(itEnd))
                {
                    return T1Hash::Null;
                }

                byte hex2 = Math::CharToHex(it.value());
                if (hex2 == byte(-1))
                {
                    return T1Hash::Null;
                }
                it.next();

                hash.bytes[i++] = hex1 + hex2;
            }

            if (i < 20)
            {
                return T1Hash::Null;
            }

            return hash;
        }

        template <typename TIter, typename TIterEnd>
        constexpr auto _parseJumpRange(TIter it, TIterEnd itEnd) const -> T1Hash
        {
            if (itEnd.compare(it).ne(Size * 2))
            {
                return T1Hash::Null;
            }

            T1Hash hash;
            for (usize i = 0; i < 20; i++)
            {
                byte hex1 = Math::CharToHex(it.value());
                if (hex1 == byte(-1))
                {
                    return T1Hash::Null;
                }

                // Left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;
                it.next();

                byte hex2 = Math::CharToHex(it.value());
                if (hex2 == byte(-1))
                {
                    return T1Hash::Null;
                }
                it.next();

                hash.bytes[i] = hex1 + hex2;
            }

            return hash;
        }

        constexpr auto _parseArr(const Char* str) const -> T1Hash
        {
            T1Hash hash;
            for (usize i = 0; i < Size * 2; i += 2)
            {
                byte hex1 = Math::CharToHex(str[i.val()]);
                if (hex1 == byte(-1))
                {
                    return T1Hash::Null;
                }

                // Left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;

                byte hex2 = Math::CharToHex(str[(i + 1).val()]);
                if (hex2 == byte(-1))
                {
                    return T1Hash::Null;
                }

                hash.bytes[i / 2] = hex1 + hex2;
            }

            return hash;
        }
    };
}
