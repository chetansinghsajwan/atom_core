#pragma once
#include "StrArgFormatters.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    class _FmtStrViewCnvter
    {
        pub cexpr fn FromFmt(_FmtStrView strv) noex -> StrView
        {
            return StrView{ Range(strv.data(), strv.size()) };
        }

        pub cexpr fn ToFmt(StrView strv) noex -> _FmtStrView
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
        pub StrView str;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <RStrFmtArgFmtable... TArgs>
    using FmtStr = _FmtFmtStr<TArgs...>;
//     class FmtStr
//     {
//         pub template <typename T>
//         ceval FmtStr(const T& strv) noex { }
//             _fmt{ _FmtStrViewCnvter().ToFmt(strv) } { }
// 
//         pub FmtStr(RunFmtStr str) noex { }
//             _fmt{ _FmtRunFmtStr{ _FmtStrViewCnvter().ToFmt(str.str) } } { }
// 
//         pub _FmtFmtStr<TArgs...> _fmt;
//     };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    class StrFmter
    {
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <typename TOut, RStrFmtArgFmtable... TArgs>
        requires ROutput<TOut, Char>
        fn FmtTo(TOut out, FmtStr<TArgs...> fmt, TArgs&&... args)
        {
            class _OutIterWrap
            {
                pub fn op++(i32) -> _OutIterWrap&
                {
                    return self;
                }

                pub fn op*() -> _OutIterWrap&
                {
                    return self;
                }

                pub fn op=(Char ch) -> _OutIterWrap&
                {
                    *out+= ch;
                    return self;
                }

                pub TOut* out;
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
        pub template <RStrFmtArgFmtable... TArgs>
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
