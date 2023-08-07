#pragma once
#include "Atom/Str.h"
#include "Atom/TTI.h"

#include "Atom/Uuid/Uuid.h"

namespace Atom::Private
{
    template <tname THash, tname THashGenerator, EUuidVersion UuidVersion>
    class UuidNameGenerator
    {
    public:
        explicit ctor UuidNameGenerator(const Uuid& nsUuid) noex:
            _nsUuid(nsUuid) { }

    public:
        fn Generate(StrView name) -> Uuid
        {
            _Reset();
            _ProcessChars(name);
            return _MakeUuid();
        }

    private:
        fn _Reset()
        {
            _hashGenerator.Reset();
            _hashGenerator.ProcessBytes(_nsUuid.bytes.Data(), 16);
        }

        fn _ProcessChars(StrView str)
        {
            for (uint32_t c : str)
            {
                _hashGenerator.ProcessByte(static_cast<byte>(c & 0xFF));

                // TODO: Check this
                // if cexpr (!TTI::IsSame<Char, char>)
                // {
                //     _hashGenerator.ProcessByte(static_cast<byte>((c >> 8) & 0xFF));
                //     _hashGenerator.ProcessByte(static_cast<byte>((c >> 16) & 0xFF));
                //     _hashGenerator.ProcessByte(static_cast<byte>((c >> 24) & 0xFF));
                // }
            }
        }

        fn _MakeUuid() -> Uuid
        {
            THash hash = _hashGenerator.Generate();

            // Variant must be EUuidVariant::RFC (0b10xxxxxx).
            hash.bytes[8] &= 0xBF;
            hash.bytes[8] |= 0x80;

            // Set version.
            // Clear out the relevant bits and apply them
            hash.bytes[6] &= 0x0F;
            hash.bytes[6] |= ((i32)UuidVersion << 4);

            Uuid uuid;
            for (usize i = 0; i < 16; i++)
            {
                uuid.bytes[i] = hash.bytes[i];
            }

            return uuid;
        }

    private:
        Uuid _nsUuid;
        THashGenerator _hashGenerator;
    };
}

namespace Atom
{
    class UuidNamespaces
    {
        // Name string is a fully-qualified domain name
        pub static cexpr Uuid Dns =
        {
            0x6b, 0xa7, 0xb8, 0x10, 0x9d, 0xad, 0x11, 0xd1,
            0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
        };

        // Name string is a URL
        pub static cexpr Uuid Url =
        {
            0x6b, 0xa7, 0xb8, 0x11, 0x9d, 0xad, 0x11, 0xd1,
            0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
        };

        // Name string is an ISO OID
        pub static cexpr Uuid Oid =
        {
            0x6b, 0xa7, 0xb8, 0x12, 0x9d, 0xad, 0x11, 0xd1,
            0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
        };

        // Name string is an X.500 DN, in DER or a text output format
        pub static cexpr Uuid X500 =
        {
            0x6b, 0xa7, 0xb8, 0x14, 0x9d, 0xad, 0x11, 0xd1,
            0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
        };
    };
}