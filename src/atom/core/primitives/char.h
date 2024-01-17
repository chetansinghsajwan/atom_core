#pragma once
#include "Byte.h"

namespace Atom
{
    using ch = char;
    using ch8 = char8_t;
    using ch16 = char16_t;
    using ch32 = char32_t;

    using _char = char;

    class Char
    {
        using This = Char;

    public:
        constexpr Char()
            : _val(0)
        {}

        constexpr Char(const This& that) = default;
        constexpr Char& operator=(const This& that) = default;

        constexpr Char(This&& that) = default;
        constexpr Char& operator=(This&& that) = default;

        constexpr Char(_char ch)
            : _val(ch)
        {}

        constexpr Char& operator=(_char ch)
        {
            _val = ch;
            return *this;
        }

        constexpr operator byte() const
        {
            return byte(_val);
        }

        constexpr ~Char() = default;

    public:
        constexpr auto eq(Char ch) const -> bool
        {
            return _val == ch._val;
        }

        constexpr auto ne(Char ch) const -> bool
        {
            return _val != ch._val;
        }

        constexpr auto lt(Char ch) const -> bool
        {
            return _val < ch._val;
        }

        constexpr auto gt(Char ch) const -> bool
        {
            return _val > ch._val;
        }

        constexpr auto le(Char ch) const -> bool
        {
            return _val <= ch._val;
        }

        constexpr auto ge(Char ch) const -> bool
        {
            return _val >= ch._val;
        }

        constexpr auto operator+(Char ch) const -> Char
        {
            return Char(_val + ch._val);
        }

        constexpr auto operator-(char ch) const -> Char
        {
            return Char(_val - ch);
        }

        constexpr auto operator+(int n) const -> Char
        {
            return Char(_val + n);
        }

    public:
        _char _val;
    };

    constexpr auto _ToStdCharPtr(const Char* ptr) -> const char*
    {
        return static_cast<const char*>(static_cast<const void*>(ptr));
    }

    constexpr auto _ToStdCharPtr(Char* ptr) -> char*
    {
        return static_cast<char*>(static_cast<void*>(ptr));
    }

    constexpr auto _FromStdCharPtr(const char* ptr) -> const Char*
    {
        return static_cast<const Char*>(static_cast<const void*>(ptr));
    }

    constexpr auto _FromStdCharPtr(char* ptr) -> Char*
    {
        return static_cast<Char*>(static_cast<void*>(ptr));
    }
}
