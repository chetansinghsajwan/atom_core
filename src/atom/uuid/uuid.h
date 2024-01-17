#pragma once
#include "atom/math.h"
#include "atom/core/static_storage.h"
#include "atom/string/string.h"
#include "atom/containers/output_req.h"

namespace atom
{
    enum class euuid_version : byte
    {
        none = 0,

        v1 = 1,
        v2 = 2,
        v3 = 3,
        v4 = 4,
        v5 = 5,

        time_based = v1,
        dce_security = v2,
        name_based_md5_ = v3,
        random_number_based = v4,
        name_based_sha1_ = v5
    };

    enum class euuid_variant : byte
    {
        ncs,
        rfc,
        microsoft,
        reserved
    };

    class uuid
    {
    public:
        static const uuid null;

    public:
        constexpr auto get_variant() const -> euuid_variant
        {
            if ((bytes[8] & 0x80) == 0x00)
                return euuid_variant::ncs;
            else if ((bytes[8] & 0xc0) == 0x80)
                return euuid_variant::rfc;
            else if ((bytes[8] & 0xe0) == 0xc0)
                return euuid_variant::microsoft;

            return euuid_variant::reserved;
        }

        constexpr auto get_version() const -> euuid_version
        {
            if ((bytes[6] & 0xf0) == 0x10)
                return euuid_version::v1;
            else if ((bytes[6] & 0xf0) == 0x20)
                return euuid_version::v2;
            else if ((bytes[6] & 0xf0) == 0x30)
                return euuid_version::v3;
            else if ((bytes[6] & 0xf0) == 0x40)
                return euuid_version::v4;
            else if ((bytes[6] & 0xf0) == 0x50)
                return euuid_version::v5;

            return euuid_version::none;
        }

        constexpr auto eq(const uuid& that) const -> bool
        {
            return bytes == that.bytes;
        }

        constexpr auto ne(const uuid& that) const -> bool
        {
            return bytes == that.bytes;
        }

        constexpr auto operator==(const uuid& that) const -> bool
        {
            return eq(that);
        }

        constexpr auto operator!=(const uuid& that) const -> bool
        {
            return ne(that);
        }

        template <typename tout>
        constexpr auto write_string(tout&& out) const
            requires(routput<tout, uchar>)
        {
            out += math::hex_to_char(bytes[0]);
            out += math::hex_to_char(bytes[1]);
            out += math::hex_to_char(bytes[2]);
            out += math::hex_to_char(bytes[3]);
            out += '-';
            out += math::hex_to_char(bytes[4]);
            out += math::hex_to_char(bytes[5]);
            out += '-';
            out += math::hex_to_char(bytes[6]);
            out += math::hex_to_char(bytes[7]);
            out += '-';
            out += math::hex_to_char(bytes[8]);
            out += math::hex_to_char(bytes[9]);
            out += '-';
            out += math::hex_to_char(bytes[10]);
            out += math::hex_to_char(bytes[11]);
            out += math::hex_to_char(bytes[12]);
            out += math::hex_to_char(bytes[13]);
            out += math::hex_to_char(bytes[14]);
            out += math::hex_to_char(bytes[15]);
        }

        constexpr auto to_string() const -> string
        {
            string out;
            write_string(out);
            return out;
        }

    public:
        static_storage<16> bytes;
    };

    constexpr inline const uuid uuid::null = uuid{ 0 };
}
