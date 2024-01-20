#pragma once
#include "atom/string.h"
// #include "atom/tti.h"

#include "atom/uuid/uuid.h"

namespace atom
{
    template <typename hash_type, typename hash_generator_type, euuid_version uuid_version>
    class _uuid_name_generator
    {
    public:
        explicit _uuid_name_generator(const uuid& ns_uuid)
            : _ns_uuid(ns_uuid)
        {}

    public:
        auto generate(string_view name) -> uuid
        {
            _reset();
            _process_string(name);
            return _make_uuid();
        }

    private:
        auto _reset()
        {
            _hash_generator.reset();
            _hash_generator.process_bytes(_ns_uuid.bytes.mem(), 16);
        }

        auto _process_string(string_view str)
        {
            for (_u32 c : str)
            {
                _hash_generator.process_byte(static_cast<byte>(c & 0xff));

                // todo: check this
                // if constexpr (!tti::is_same<uchar, char>)
                // {
                //     _hash_generator.process_byte(static_cast<byte>((c >> 8) & 0xff));
                //     _hash_generator.process_byte(static_cast<byte>((c >> 16) & 0xff));
                //     _hash_generator.process_byte(static_cast<byte>((c >> 24) & 0xff));
                // }
            }
        }

        auto _make_uuid() -> uuid
        {
            hash_type hash = _hash_generator.generate();

            // variant must be euuid_variant::rfc (0b10xxxxxx).
            hash.bytes[8] &= 0xbf;
            hash.bytes[8] |= 0x80;

            // set version.
            // clear out the relevant bits and apply them
            hash.bytes[6] &= 0x0f;
            hash.bytes[6] |= ((_i32)uuid_version << 4);

            uuid uuid;
            for (usize i = 0; i < 16; i++)
            {
                uuid.bytes[i] = hash.bytes[i];
            }

            return uuid;
        }

    private:
        uuid _ns_uuid;
        hash_generator_type _hash_generator;
    };
}

namespace atom
{
    class uuid_namespaces
    {
    public:
        // name string is a fully-qualified domain name
        static constexpr uuid dns = {
            {0x6b, 0xa7, 0xb8, 0x10, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4,
             0x30, 0xc8}
        };

        // name string is a url
        static constexpr uuid url = {
            {0x6b, 0xa7, 0xb8, 0x11, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4,
             0x30, 0xc8}
        };

        // name string is an iso oid
        static constexpr uuid oid = {
            {0x6b, 0xa7, 0xb8, 0x12, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4,
             0x30, 0xc8}
        };

        // name string is an x.500 dn, in der or a text output format
        static constexpr uuid x500 = {
            {0x6b, 0xa7, 0xb8, 0x14, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4,
             0x30, 0xc8}
        };
    };
}
