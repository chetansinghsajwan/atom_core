#pragma once
#include "Atom/String.h"

#include "Atom/Fmt/Formatters.h"

namespace Atom::Fmt
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <RFormattable... TArgs>
    using FormatString = _TFormatString<TArgs...>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    using RuntimeFormatString = _TRuntimeFormatString;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    inline RuntimeFormatString Runtime(StringView fmt) noexcept
    {
        return RuntimeFormatString{ fmt };
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <RBackInsertable<Char> TBackInsertable, RFormattable... TArgs>
    void FormatTo(TBackInsertable& out, FormatString<TArgs...> in_fmt, TArgs&&... in_args)
    {
        struct OutputIterator
        {
            OutputIterator& operator ++ (int)
            {
                return *this;
            }

            OutputIterator& operator * ()
            {
                return *this;
            }

            OutputIterator& operator = (Char ch)
            {
                out->InsertBack(ch);
                return *this;
            }

            TBackInsertable* out;
        };

        fmt::detail::iterator_buffer<OutputIterator, Char> buf{ OutputIterator{ &out } };

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

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <RFormattable... TArgs>
    String Format(FormatString<TArgs...> in_fmt, TArgs&&... in_args)
    {
        String out;
        StringWrapper outWrapper { out };
        FormatTo(outWrapper, in_fmt, FORWARD(in_args)...);

        return out;
    }
}