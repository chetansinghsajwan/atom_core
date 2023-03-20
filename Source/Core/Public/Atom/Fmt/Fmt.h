#pragma once
#include "fmt/format.h"

#include "Atom/String.h"
#include "Atom/TTI.h"

namespace Atom::Fmt
{
    template <typename TElement, typename TBackInsertable>
    concept RBackInsertable = requires(TBackInsertable insertable)
    {
        insertable.InsertBack(TElement());
    };

    template <typename... TArgs>
    using FormatString = fmt::basic_format_string<Char, fmt::type_identity_t<TArgs>...>;

    template <typename TBackInsertable, typename... TArgs>
        requires RBackInsertable<Char, TBackInsertable>
    void FormatTo(TBackInsertable& out, FormatString<TArgs...> fmt, TArgs&&... args)
    {
        struct Wrapper
        {
            struct Input
            {
                void operator = (Char ch)
                {
                    out.InsertBack(ch);
                }

                TBackInsertable& out;
            };

            Wrapper(TBackInsertable* in_out) noexcept:
                out{ in_out } { }

            Wrapper& operator ++ (int)
            {
                return *this;
            }

            auto operator * ()
            {
                return Input{ *out };
            }

            TBackInsertable* out;
        };

        fmt::detail::iterator_buffer<Wrapper, Char> buf{ Wrapper{ &out } };
        fmt::detail::vformat_to<Char>(buf, fmt,
            fmt::make_format_args<fmt::buffer_context<Char>>(FORWARD(args)...),
            fmt::detail::locale_ref{});
    }

    template <typename... TArgs>
    String Format(FormatString<TArgs...> fmt, TArgs&&... args)
    {
        struct StringWrapper
        {
            constexpr StringWrapper(String& in_str) noexcept:
                str{ in_str } { }

            void InsertBack(Char ch)
            {
                str.push_back(ch);
            }

            String& str;
        };

        String out;
        StringWrapper outWrapper = out;
        FormatTo(outWrapper, fmt, FORWARD(args)...);

        return out;
    }
}