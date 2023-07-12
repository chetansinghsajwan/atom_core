#pragma once
#include "StrArgFormatters.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    struct _FmtStrViewCnvter
    {
        cexpr StrView FromFmt(_FmtStrView strv) noex
        {
            return StrView{ Range(strv.data(), strv.size()) };
        }

        cexpr _FmtStrView ToFmt(StrView strv) noex
        {
            return _FmtStrView{ strv.Data(), strv.Count() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Wrapper over {StrView} to represent format string. This is done to avoid compile 
    /// time checks.
    /// --------------------------------------------------------------------------------------------
    struct RunFmtStr
    {
        StrView str;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <RStrFmtArgFmtable... TArgs>
    using FmtStr = _FmtFmtStr<TArgs...>;
//     struct FmtStr
//     {
//         template <tname T>
//         ceval FmtStr(const T& strv) noex { }
//             _fmt{ _FmtStrViewCnvter().ToFmt(strv) } { }
// 
//         FmtStr(RunFmtStr str) noex { }
//             _fmt{ _FmtRunFmtStr{ _FmtStrViewCnvter().ToFmt(str.str) } } { }
// 
//         _FmtFmtStr<TArgs...> _fmt;
//     };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    struct StrFmter
    {
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <tname TOut, RStrFmtArgFmtable... TArgs>
        requires ROutput<TOut, Char>
        void FmtTo(TOut out, FmtStr<TArgs...> fmt, TArgs&&... args)
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
        Str Fmt(FmtStr<TArgs...> fmt, TArgs&&... args)
        {
            Str out;
            FmtTo(out, fmt, fwd(args)...);

            return out;
        }
    };
}

#define ATOM_STR_FMT(fmt, ...) \
    ::Atom::StrFmter(TEXT(fmt), __VA_ARGS__)
