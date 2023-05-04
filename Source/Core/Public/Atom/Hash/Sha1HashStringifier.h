#include <bit>
#include "Atom/Hash/Sha1Hash.h"
#include "Atom/Hash/Sha1HashGenerator.h"
#include "Atom/Hash/Sha1HashParser.h"
#include "Atom/String/String.h"
#include "Atom/String/StringConverter.h"

namespace Atom
{
    struct Sha1HashStringifier
    {
        constexpr String ToString(const Sha1Hash& hash) noexcept
        {
            String str;
            WriteString(hash, str);
            return str;
        };

        constexpr void WriteString(const Sha1Hash& hash, ROutputWriter<Char> auto&& out) noexcept
        {
            for (byte b : hash.bytes)
            {
                Math::HexToChar(b, out);
            }
        }
    };

    template < >
    struct StringConverter<Sha1Hash>
    {
        constexpr String Convert(const Sha1Hash& hash) noexcept
        {
            return Sha1HashStringifier().ToString(hash);
        }

        constexpr void Convert(const Sha1Hash& hash, ROutputWriter<Char> auto out) noexcept
        {
            return Sha1HashStringifier().WriteString(hash, out);
        }
    };
}