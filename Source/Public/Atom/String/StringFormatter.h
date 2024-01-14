#pragma once
#include "StringArgFormatters.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class _FmtStringViewCnvter
    {
    public:
        constexpr auto FromFmt(_FmtStringView str) -> StringView
        {
            const Char* begin = _FromStdCharPtr(str.data());
            usize count = str.size();
            return StringView{ MakeRange(begin, count) };
        }

        constexpr auto ToFmt(StringView str) -> _FmtStringView
        {
            const char* begin = _ToStdCharPtr(str.data().unwrap());
            const _usize count = str.count().unwrap();
            return _FmtStringView{ begin, count };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Wrapper over {StringView} to represent format string. This is done to avoid compile
    /// time checks.
    /// --------------------------------------------------------------------------------------------
    class RunFmtString
    {
    public:
        StringView str;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <RStringFmtArgFmtable... TArgs>
    using FmtString = _FmtFmtString<TArgs...>;

    //     class FmtString
    //     {
    //     public:
    //         template <typename T>
    //         consteval FmtString(const T& strv) { }
    //             _fmt{ _FmtStringViewCnvter().ToFmt(strv) } { }
    //
    //         FmtString(RunFmtString str) { }
    //             _fmt{ _FmtRunFmtString{ _FmtStringViewCnvter().ToFmt(str.str) } } { }
    //
    //     public:
    //         _FmtFmtString<TArgs...> _fmt;
    //     };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class StringFmter
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TOut, RStringFmtArgFmtable... TArgs>
        auto FmtTo(TOut out, FmtString<TArgs...> fmt, TArgs&&... args)
            requires ROutput<TOut, Char>
        {
            class _OutIterWrap
            {
            public:
                auto operator++(int) -> _OutIterWrap&
                {
                    return *this;
                }

                auto operator*() -> _OutIterWrap&
                {
                    return *this;
                }

                auto operator=(char ch) -> _OutIterWrap&
                {
                    *out += Char(ch);
                    return *this;
                }

            public:
                TOut* out;
            };

            fmt::detail::iterator_buffer<_OutIterWrap, char> buf{ _OutIterWrap{ &out } };

            try
            {
                fmt::detail::vformat_to<char>(buf, fmt,
                    fmt::make_format_args<fmt::buffer_context<char>>(args...),
                    fmt::detail::locale_ref{});
            }
            catch (const _FmtFmtEx& err)
            {
                throw StringFmtErr(err);
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <RStringFmtArgFmtable... TArgs>
        auto Fmt(FmtString<TArgs...> fmt, TArgs&&... args) -> String
        {
            String out;
            FmtTo(out, fmt, forward<TArgs>(args)...);

            return out;
        }
    };
}
