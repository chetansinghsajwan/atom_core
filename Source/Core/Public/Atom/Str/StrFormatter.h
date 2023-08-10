#pragma once
#include "StrArgFormatters.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    class _FmtStrViewCnvter
    {
    public:
        constexpr fn FromFmt(_FmtStrView strv) -> StrView
        {
            return StrView{ Range(strv.data(), strv.size()) };
        }

        constexpr fn ToFmt(StrView strv) -> _FmtStrView
        {
            return _FmtStrView{ strv.data(), strv.count() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Wrapper over {StrView} to represent format string. This is done to avoid compile 
    /// time checks.
    /// --------------------------------------------------------------------------------------------
    class RunFmtStr
    {
    public:
        StrView str;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <RStrFmtArgFmtable... TArgs>
    using FmtStr = _FmtFmtStr<TArgs...>;
//     class FmtStr
//     {
//     public:
//         template <typename T>
//         consteval FmtStr(const T& strv) { }
//             _fmt{ _FmtStrViewCnvter().ToFmt(strv) } { }
// 
//         FmtStr(RunFmtStr str) { }
//             _fmt{ _FmtRunFmtStr{ _FmtStrViewCnvter().ToFmt(str.str) } } { }
// 
//     public:
//         _FmtFmtStr<TArgs...> _fmt;
//     };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    class StrFmter
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TOut, RStrFmtArgFmtable... TArgs>
        requires ROutput<TOut, Char>
        fn FmtTo(TOut out, FmtStr<TArgs...> fmt, TArgs&&... args)
        {
            class _OutIterWrap
            {
            public:
                fn operator++(i32) -> _OutIterWrap&
                {
                    return self;
                }

                fn operator*() -> _OutIterWrap&
                {
                    return self;
                }

                fn operator=(Char ch) -> _OutIterWrap&
                {
                    *out+= ch;
                    return self;
                }

            public:
                TOut* out;
            };

            fmt::detail::iterator_buffer<_OutIterWrap, Char> buf{ _OutIterWrap{ &out } };

            try
            {
                fmt::detail::vformat_to<Char>(buf, fmt,
                    fmt::make_format_args<fmt::buffer_context<Char>>(fwd(args)...),
                    fmt::detail::locale_ref{});
            }
            catch (const _FmtFmtEx& err)
            {
                throw StrFmtEx(err);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RStrFmtArgFmtable... TArgs>
        fn Fmt(FmtStr<TArgs...> fmt, TArgs&&... args) -> Str
        {
            Str out;
            FmtTo(out, fmt, fwd(args)...);

            return out;
        }
    };
}

#define ATOM_STR_FMT(fmt, ...) \
    ::Atom::StrFmter(fmt, __VA_ARGS__)
