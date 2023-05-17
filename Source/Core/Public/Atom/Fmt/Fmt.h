#pragma once
#include "Atom/String.h"

#include "Atom/Fmt/Formatters.h"

namespace Atom::Fmt
{
    using FMT_StringView = _TStringView;

    template <typename... TArgs>
    using FMT_FormatString = _TFormatString<TArgs...>;
    using FMT_RuntimeFormatString = _TRuntimeFormatString;

    /// 
    /// --------------------------------------------------------------------------------------------
    struct FMT_StringViewConverter
    {
        constexpr StringView FromFMT(FMT_StringView strv) noexcept
        {
            return StringView{ strv.data(), strv.size() };
        }

        constexpr FMT_StringView ToFMT(StringView strv) noexcept
        {
            return FMT_StringView{ strv.Data(), strv.Count() };
        }
    };

    /// Wrapper over {StringView} to represent format string. This is done to avoid compile 
    /// time checks.
    /// --------------------------------------------------------------------------------------------
    struct RuntimeFormatString
    {
        StringView str;
    };

    /// 
    /// --------------------------------------------------------------------------------------------
    template <RFormattable... TArgs>
    using FormatString = _TFormatString<TArgs...>;
//     struct FormatString
//     {
//         template <typename T>
//         consteval FormatString(const T& strv) noexcept { }
//             _fmt{ FMT_StringViewConverter().ToFMT(strv) } { }
// 
//         FormatString(RuntimeFormatString str) noexcept { }
//             _fmt{ FMT_RuntimeFormatString{ FMT_StringViewConverter().ToFMT(str.str) } } { }
// 
//         _TFormatString<TArgs...> _fmt;
//     };

    /// 
    /// --------------------------------------------------------------------------------------------
    inline RuntimeFormatString Runtime(StringView fmt) noexcept
    {
        return RuntimeFormatString{ fmt };
    }

    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TOut, RFormattable... TArgs>
    requires ROutput<TOut, Char>
    void FormatTo(TOut out, FormatString<TArgs...> fmt, TArgs&&... args)
    {
        struct OutItWrap
        {
            OutItWrap& operator ++ (int)
            {
                return *this;
            }

            OutItWrap& operator * ()
            {
                return *this;
            }

            OutItWrap& operator = (Char ch)
            {
                *out+= ch;
                return *this;
            }

            TOut* out;
        };

        fmt::detail::iterator_buffer<OutItWrap, Char> buf{ OutItWrap{ &out } };

        try
        {
            fmt::detail::vformat_to<Char>(buf, fmt,
                fmt::make_format_args<fmt::buffer_context<Char>>(FORWARD(args)...),
                fmt::detail::locale_ref{});
        }
        catch (const _TFormatError& err)
        {
            throw FormatException(err);
        }
    }

    /// 
    /// --------------------------------------------------------------------------------------------
    template <RFormattable... TArgs>
    String Format(FormatString<TArgs...> fmt, TArgs&&... args)
    {
        String out;
        FormatTo(out, fmt, FORWARD(args)...);

        return out;
    }
}