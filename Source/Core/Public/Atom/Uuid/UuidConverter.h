#pragma once
#include "Atom/String.h"

#include "Atom/Uuid/Uuid.h"

namespace Atom
{
    namespace CharUtils
    {
        constexpr Char HexToChar(Char ch) noexcept
        {
            if (ch >= TEXT('0') && ch <= TEXT('9'))
                return ch - TEXT('0');
            if (ch >= TEXT('a') && ch <= TEXT('f'))
                return 10 + ch - TEXT('a');
            if (ch >= TEXT('A') && ch <= TEXT('F'))
                return 10 + ch - TEXT('A');

            return 0;
        }

        constexpr bool IsHex(Char ch) noexcept
        {
            return (ch >= TEXT('0') && ch <= TEXT('9')) ||
                (ch >= TEXT('a') && ch <= TEXT('f')) ||
                (ch >= TEXT('A') && ch <= TEXT('F'));
        }
    }

    template <typename TElement, typename TBackInsertable>
    concept RBackInsertable = requires(TBackInsertable insertable)
    {
        insertable.InsertBack(TElement());
    };

    class UuidConverter
    {
        static constexpr Uuid FromString(StringView in_str) noexcept
        {
            bool firstDigit = true;
            usize hasBraces = 0;
            usize index = 0;

            Uuid uuid = { 0 };

            if (in_str.empty())
                return Uuid{};

            if (in_str.front() == TEXT('{'))
                hasBraces = 1;

            if (hasBraces && in_str.back() != TEXT('}'))
                return Uuid{};

            for (usize i = hasBraces; i < in_str.size() - hasBraces; ++i)
            {
                if (in_str[i] == TEXT('-')) continue;

                if (index >= 16 || !CharUtils::IsHex(in_str[i]))
                {
                    return Uuid{};
                }

                if (firstDigit)
                {
                    uuid.bytes[index] = static_cast<byte>(CharUtils::HexToChar(in_str[i]) << 4);
                    firstDigit = false;
                }
                else
                {
                    uuid.bytes[index] = static_cast<byte>(uuid.bytes[index] | CharUtils::HexToChar(in_str[i]));
                    firstDigit = true;
                    index++;
                }
            }

            if (index < 16)
            {
                return Uuid{};
            }

            return uuid;
        }

        template <typename TBackInsertable>
            requires RBackInsertable<Char, TBackInsertable>
        static constexpr void ToString(const Uuid& in_uuid, TBackInsertable& out_str) noexcept
        {
            for (usize i = 0, index = 0; i < 36; ++i)
            {
                if (i == 8 || i == 13 || i == 18 || i == 23)
                {
                    out_str.InsertBack(TEXT('-'));
                    continue;
                }

                out_str.InsertBack(s_uuidChars[in_uuid.bytes[index] >> 4 & 0x0f]);
                out_str.InsertBack(s_uuidChars[in_uuid.bytes[index] & 0x0f]);
                index++;
            }
        }

        static constexpr String ToString(const Uuid& in_uuid) noexcept
        {
            struct Wrapper
            {
                constexpr void InsertBack(Char ch)
                {
                    str.push_back(ch);
                }

                String& str;
            };

            String outStr{ 36 };
            Wrapper wrapper{ outStr };
            ToString(in_uuid, wrapper);
            return outStr;
        }

    protected:
        static constexpr Char s_uuidChars[] = TEXT("0123456789abcdef");
    };
}