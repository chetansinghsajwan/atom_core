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

        constexpr StringView ToFMT(RFwdIt<Char> auto in) noexcept
        {
            return StringView{ in.begin(), in.end() };
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
    template <ROutputWriter<Char> TOutput, RFormattable... TArgs>
    void FormatTo(TOutput out, FormatString<TArgs...> in_fmt, TArgs&&... in_args)
    {
        struct _OutputIteratorWrapper
        {
            _OutputIteratorWrapper& operator ++ (int)
            {
                return *this;
            }

            _OutputIteratorWrapper& operator * ()
            {
                return *this;
            }

            _OutputIteratorWrapper& operator = (Char ch)
            {
                *out+= ch;
                return *this;
            }

            TOutput* out;
        };

        fmt::detail::iterator_buffer<_OutputIteratorWrapper, Char> buf{
            _OutputIteratorWrapper{ &out } };

        try
        {
            fmt::detail::vformat_to<Char>(buf, in_fmt,
                fmt::make_format_args<fmt::buffer_context<Char>>(FORWARD(in_args)...),
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
    String Format(FormatString<TArgs...> in_fmt, TArgs&&... in_args)
    {
        String out;
        FormatTo(out, in_fmt, FORWARD(in_args)...);

        return out;
    }
}