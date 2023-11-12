#pragma once
#include "Atom/Math.h"
#include "Atom/Core/StaticStorage.h"
#include "Atom/String/String.h"
#include "Atom/Containers/OutputReq.h"

namespace Atom
{
    enum class EUuidVersion : byte
    {
        None = 0,

        V1 = 1,
        V2 = 2,
        V3 = 3,
        V4 = 4,
        V5 = 5,

        TimeBased = V1,
        DceSecurity = V2,
        NameBasedMD5 = V3,
        RandomNumberBased = V4,
        NameBasedSHA1 = V5
    };

    enum class EUuidVariant : byte
    {
        NCS,
        RFC,
        Microsoft,
        Reserved
    };

    class Uuid
    {
    public:
        static const Uuid Null;

    public:
        constexpr auto getVariant() const -> EUuidVariant
        {
            if ((bytes[8] & 0x80) == 0x00)
                return EUuidVariant::NCS;
            else if ((bytes[8] & 0xC0) == 0x80)
                return EUuidVariant::RFC;
            else if ((bytes[8] & 0xE0) == 0xC0)
                return EUuidVariant::Microsoft;

            return EUuidVariant::Reserved;
        }

        constexpr auto getVersion() const -> EUuidVersion
        {
            if ((bytes[6] & 0xF0) == 0x10)
                return EUuidVersion::V1;
            else if ((bytes[6] & 0xF0) == 0x20)
                return EUuidVersion::V2;
            else if ((bytes[6] & 0xF0) == 0x30)
                return EUuidVersion::V3;
            else if ((bytes[6] & 0xF0) == 0x40)
                return EUuidVersion::V4;
            else if ((bytes[6] & 0xF0) == 0x50)
                return EUuidVersion::V5;

            return EUuidVersion::None;
        }

        constexpr auto eq(const Uuid& that) const -> bool
        {
            return bytes == that.bytes;
        }

        constexpr auto ne(const Uuid& that) const -> bool
        {
            return bytes == that.bytes;
        }

        constexpr auto operator==(const Uuid& that) const -> bool
        {
            return eq(that);
        }

        constexpr auto operator!=(const Uuid& that) const -> bool
        {
            return ne(that);
        }

        template <typename TOut>
        constexpr auto writeString(TOut&& out) const
            requires(ROutput<TOut, Char>)
        {
            out += Math::HexToChar(bytes[0]);
            out += Math::HexToChar(bytes[1]);
            out += Math::HexToChar(bytes[2]);
            out += Math::HexToChar(bytes[3]);
            out += '-';
            out += Math::HexToChar(bytes[4]);
            out += Math::HexToChar(bytes[5]);
            out += '-';
            out += Math::HexToChar(bytes[6]);
            out += Math::HexToChar(bytes[7]);
            out += '-';
            out += Math::HexToChar(bytes[8]);
            out += Math::HexToChar(bytes[9]);
            out += '-';
            out += Math::HexToChar(bytes[10]);
            out += Math::HexToChar(bytes[11]);
            out += Math::HexToChar(bytes[12]);
            out += Math::HexToChar(bytes[13]);
            out += Math::HexToChar(bytes[14]);
            out += Math::HexToChar(bytes[15]);
        }

        constexpr auto toString() const -> String
        {
            String out;
            writeString(out);
            return out;
        }

    public:
        StaticStorage<16> bytes;
    };

    constexpr inline const Uuid Uuid::Null = Uuid{ 0 };
}
