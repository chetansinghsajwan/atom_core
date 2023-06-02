#pragma once
#include "StringArgFormatters.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    struct _FmtStringViewConverter
    {
        constexpr StringView FromFmt(_FmtStringView strv) noexcept
        {
            return StringView{ strv.data(), strv.size() };
        }

        constexpr _FmtStringView ToFmt(StringView strv) noexcept
        {
            return _FmtStringView{ strv.Data(), strv.Count() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Wrapper over {StringView} to represent format string. This is done to avoid compile 
    /// time checks.
    /// --------------------------------------------------------------------------------------------
    struct RuntimeFormatString
    {
        StringView str;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <RStringArgFormattable... TArgs>
    using FormatString = _FmtFormatString<TArgs...>;
//     struct FormatString
//     {
//         template <typename T>
//         consteval FormatString(const T& strv) noexcept { }
//             _fmt{ _FmtStringViewConverter().ToFmt(strv) } { }
// 
//         FormatString(RuntimeFormatString str) noexcept { }
//             _fmt{ _FmtRuntimeFormatString{ _FmtStringViewConverter().ToFmt(str.str) } } { }
// 
//         _FmtFormatString<TArgs...> _fmt;
//     };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    struct StringFormatter
    {
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TOut, RStringArgFormattable... TArgs>
        requires ROutput<TOut, Char>
        void FormatTo(TOut out, FormatString<TArgs...> fmt, TArgs&&... args)
        {
            struct _OutIterWrap
            {
                _OutIterWrap& operator ++ (int)
                {
                    return *this;
                }

                _OutIterWrap& operator * ()
                {
                    return *this;
                }

                _OutIterWrap& operator = (Char ch)
                {
                    *out+= ch;
                    return *this;
                }

                TOut* out;
            };

            fmt::detail::iterator_buffer<_OutIterWrap, Char> buf{ _OutIterWrap{ &out } };

            try
            {
                fmt::detail::vformat_to<Char>(buf, fmt,
                    fmt::make_format_args<fmt::buffer_context<Char>>(FORWARD(args)...),
                    fmt::detail::locale_ref{});
            }
            catch (const _FmtFormatError& err)
            {
                throw StringFormatException(err);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RStringArgFormattable... TArgs>
        String Format(FormatString<TArgs...> fmt, TArgs&&... args)
        {
            String out;
            FormatTo(out, fmt, FORWARD(args)...);

            return out;
        }
    };
}

#define ATOM_STR_FMT(fmt, ...) \
    ::Atom::StringFormatter(TEXT(fmt), __VA_ARGS__)
