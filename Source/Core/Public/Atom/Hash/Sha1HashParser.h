#pragma once
#include "Sha1Hash.h"
#include "Atom/Math.h"
#include "Atom/Exceptions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Sha1 Hash Parser.
    /// --------------------------------------------------------------------------------------------
    class Sha1HashParser
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize count> 
        requires (count == 41) // 40 (each char represents one hex value as 20 bytes represent 
                               // 40 hex values) + 1 (null terminator)
        constexpr Sha1Hash Parse(const Char(&str)[count]) const noexcept
        {
            Sha1Hash hash;
            for (usize i = 0; i < 40; i += 2)
            {
                byte hex1 = Math::CharToHex(str[i + 0]);
                if (hex1 == -1)
                {
                    return Sha1Hash::Null;
                }

                // Left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;

                byte hex2 = Math::CharToHex(str[i + 1]);
                if (hex2 == -1)
                {
                    return Sha1Hash::Null;
                }

                hash.bytes[i / 2] = hex1 + hex2;
            }

            return hash;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInput>
        requires RInputIterator<TInput, Char>
        constexpr Sha1Hash Parse(TInput in) const noexcept
        {
            Sha1Hash hash;
            usize i = 0;
            while (in.HasNext())
            {
                if (i > 20)
                {
                    return Sha1Hash::Null;
                }

                byte hex1 = Math::CharToHex(in.Get());
                if (hex1 == -1)
                {
                    return Sha1Hash::Null;
                }

                // Left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;

                if (!in.Next())
                {
                    return Sha1Hash::Null;
                }

                byte hex2 = Math::CharToHex(in.Get());
                if (hex2 == -1)
                {
                    return Sha1Hash::Null;
                }
                in.Next();

                hash.bytes[i++] = hex1 + hex2;
            }

            if (i < 20)
            {
                return Sha1Hash::Null;
            }

            return hash;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInput>
        requires RDirectIterator<TInput, Char>
        constexpr Sha1Hash Parse(TInput in) const noexcept
        {
            if (in.Range() != 40)
            {
                return Sha1Hash::Null;
            }

            Sha1Hash hash;
            for (usize i = 0; i < 20; i++)
            {
                byte hex1 = Math::CharToHex(in.Get());
                if (hex1 == -1)
                {
                    return Sha1Hash::Null;
                }

                // Left shift 4 bits to make space for next 4 bits.
                hex1 = hex1 << 4;
                in.Next();

                byte hex2 = Math::CharToHex(in.Get());
                if (hex2 == -1)
                {
                    return Sha1Hash::Null;
                }
                in.Next();

                hash.bytes[i] = hex1 + hex2;
            }

            return hash;
        }
    };
}