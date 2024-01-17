#pragma once
#include "byte.h"

namespace atom
{
    using _char = char;

    class uchar
    {
        using this_type = uchar;

    public:
        constexpr uchar()
            : _val(0)
        {}

        constexpr uchar(const this_type& that) = default;
        constexpr uchar& operator=(const this_type& that) = default;

        constexpr uchar(this_type&& that) = default;
        constexpr uchar& operator=(this_type&& that) = default;

        constexpr uchar(_char ch)
            : _val(ch)
        {}

        constexpr uchar& operator=(_char ch)
        {
            _val = ch;
            return *this;
        }

        constexpr operator byte() const
        {
            return byte(_val);
        }

        constexpr ~uchar() = default;

    public:
        constexpr auto eq(uchar ch) const -> bool
        {
            return _val == ch._val;
        }

        constexpr auto ne(uchar ch) const -> bool
        {
            return _val != ch._val;
        }

        constexpr auto lt(uchar ch) const -> bool
        {
            return _val < ch._val;
        }

        constexpr auto gt(uchar ch) const -> bool
        {
            return _val > ch._val;
        }

        constexpr auto le(uchar ch) const -> bool
        {
            return _val <= ch._val;
        }

        constexpr auto ge(uchar ch) const -> bool
        {
            return _val >= ch._val;
        }

        constexpr auto operator+(uchar ch) const -> uchar
        {
            return uchar(_val + ch._val);
        }

        constexpr auto operator-(uchar ch) const -> uchar
        {
            return uchar(_val - ch);
        }

        constexpr auto operator+(int n) const -> uchar
        {
            return uchar(_val + n);
        }

    public:
        _char _val;
    };

    constexpr auto _to_std_char_ptr(const uchar* ptr) -> const char*
    {
        return static_cast<const char*>(static_cast<const void*>(ptr));
    }

    constexpr auto _to_std_char_ptr(uchar* ptr) -> char*
    {
        return static_cast<char*>(static_cast<void*>(ptr));
    }

    constexpr auto _from_std_char_ptr(const char* ptr) -> const uchar*
    {
        return static_cast<const uchar*>(static_cast<const void*>(ptr));
    }

    constexpr auto _from_std_char_ptr(char* ptr) -> uchar*
    {
        return static_cast<uchar*>(static_cast<void*>(ptr));
    }
}
