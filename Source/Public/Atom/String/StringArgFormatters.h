#pragma once
#include "fmt/format.h"

#include "String.h"
#include "StringConverter.h"
#include "StringViewConverter.h"

#include "Atom/Exceptions.h"
#include "Atom/TTI.h"

namespace Atom
{
    using _FmtFmtParseCtx = ::fmt::basic_format_parse_context<Char>;
    using _FmtFmtParseCtxIter = typename _FmtFmtParseCtx::iterator;

    using _FmtFmtCtxOut = ::fmt::detail::buffer_appender<Char>;
    using _FmtFmtCtx = ::fmt::basic_format_context<_FmtFmtCtxOut, Char>;

    template <typename... TArgs>
    using _FmtFmtString = ::fmt::basic_format_string<Char, fmt::type_identity_t<TArgs>...>;
    using _FmtRunFmtString = ::fmt::runtime_format_string<Char>;
    using _FmtStringView = ::fmt::basic_string_view<Char>;

    template <typename T>
    using _FmtFmter = ::fmt::formatter<T, Char>;

    using _FmtFmtEx = ::fmt::format_error;

    /// --------------------------------------------------------------------------------------------
    /// Err thrown during formatting error.
    /// --------------------------------------------------------------------------------------------
    class StringFmtErr: public Err
    {
    public:
        StringFmtErr(StdStringView msg):
            Err{ msg } {}

    public:
        StringFmtErr(const _FmtFmtEx& fmtEx):
            Err{ fmtEx.what() } {}
    };

    /// --------------------------------------------------------------------------------------------
    /// Context to parse format string.
    /// --------------------------------------------------------------------------------------------
    class StringFmtParseCtx
    {
    public:
        constexpr StringFmtParseCtx(_FmtFmtParseCtx& fmtCtx): _fmtCtx{ fmtCtx } {}

    public:
        constexpr auto GetRange() -> StringView
        {
            auto range = Range(_fmtCtx.begin(), _fmtCtx.end());
            return StringView(range);
        }

        constexpr auto AdvanceTo(ArrayIter<Char> it)
        {
            _fmtCtx.advance_to(it.data());
        }

    public:
        _FmtFmtParseCtx& _fmtCtx;
    };

    /// --------------------------------------------------------------------------------------------
    /// Context to write formatted string.
    /// --------------------------------------------------------------------------------------------
    class StringFmtCtx
    {
    public:
        constexpr StringFmtCtx(_FmtFmtCtx& fmtCtx): _fmtCtx{ fmtCtx } {}

        auto Write(Char ch)
        {
            auto out = _fmtCtx.out();
            *out++ = ch;
            _fmtCtx.advance_to(mov(out));
        }

        template <typename TRange>
        auto Write(const TRange& chars)
            requires(RRangeOf<TRange, Char>)
        {
            auto out = _fmtCtx.out();
            for (Char ch : chars)
            {
                *out++ = ch;
            }

            _fmtCtx.advance_to(mov(out));
        }

    public:
        _FmtFmtCtx& _fmtCtx;
    };

    /// --------------------------------------------------------------------------------------------
    /// Parses and writes string representation for types according to the specified format
    /// specifiers.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class StringFmtArgFmterImpl;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    using StringFmtArgFmter = StringFmtArgFmterImpl<TTI::TRemoveCVRef<T>>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TFmter} is {StringFmtArgFmter} for type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TFmter, typename T>
    concept RStringFmtArgFmter =
        requires(TFmter fmter, T arg, StringFmtParseCtx parseCtx, StringFmtCtx ctx) {
            fmter.Parse(parseCtx);
            fmter.Fmt(arg, ctx);
        };

    /// --------------------------------------------------------------------------------------------
    /// StringArgFmtable refers to a type for which exists a valid {StringFmtArgFmter<T>} specialization
    /// which satisfies {RStringFmtArgFmter<StringFmtArgFmter<T>, T>} requirement.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is {StringArgFmtable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RStringFmtArgFmtable = RStringFmtArgFmter<StringFmtArgFmter<T>, T>;
}

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// {StringFmtArgFmter} specialization for {StringView}.
    ///
    /// @INTERNAL
    /// Uses {fmt::fmter<fmt::string_view>} specialization.
    /// --------------------------------------------------------------------------------------------
    template <>
    class StringFmtArgFmterImpl<StringView>
    {
    public:
        constexpr auto Parse(StringFmtParseCtx& ctx)
        {
            _FmtFmtParseCtx& fmtCtx = ctx._fmtCtx;

            fmtCtx.advance_to(_fmtFmter.parse(fmtCtx));
        }

        auto Fmt(StringView str, StringFmtCtx& ctx)
        {
            _FmtFmtCtx& fmtCtx = ctx._fmtCtx;

            _FmtStringView fmt_str{ str.data(), str.count() };
            fmtCtx.advance_to(_fmtFmter.format(fmt_str, fmtCtx));
        }

    public:
        _FmtFmter<_FmtStringView> _fmtFmter;
    };

    static_assert(RStringFmtArgFmtable<StringView>, "StringView is not formattable.");

    /// --------------------------------------------------------------------------------------------
    /// {StringFmtArgFmter} specialization for {Char} arr.
    /// --------------------------------------------------------------------------------------------
    template <usize N>
    class StringFmtArgFmterImpl<Char[N]>: public StringFmtArgFmter<StringView>
    {
    public:
        auto Fmt(const Char (&chars)[N], StringFmtCtx& ctx)
        {
            StringView str{ chars, N };
            StringFmtArgFmter<StringView>::Fmt(str, ctx);
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {StringFmtArgFmter} specialization for types which satisfy {RStringViewConvertible} requirement.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
        requires(RStringViewConvertible<T>)
    class StringFmtArgFmterImpl<T>: public StringFmtArgFmter<StringView>
    {
    public:
        auto Fmt(const T& in, StringFmtCtx& ctx)
        {
            StringFmtArgFmter<StringView>::Fmt(convter.Convert(in), ctx);
        }

    public:
        StringViewConverter<T> convter;
    };

    template <typename T>
    class _FmtFmtfierFilter
    {
    public:
        static constexpr bool Enable = true;
    };

    template <usize N>
    class _FmtFmtfierFilter<Char[N]>
    {
    public:
        static constexpr bool Enable = false;
    };
}

namespace fmt
{
    /// --------------------------------------------------------------------------------------------
    /// @INTERNAL
    ///
    /// {fmt::fmter} specialization for all types that implement {Atom::StringFmtArgFmter}.
    /// {fmt} uses this type and users specialize {Atom::StringFmtArgFmter}.
    ///
    /// This is specialized for {Char} character type only as {Atom} uses that type for
    /// character representation.
    /// --------------------------------------------------------------------------------------------
    template <Atom::RStringFmtArgFmtable T>
        requires(Atom::_FmtFmtfierFilter<T>::Enable)
    class formatter<T, Atom::Char>
    {
    public:
        constexpr auto parse(Atom::_FmtFmtParseCtx& fmtCtx) -> Atom::_FmtFmtParseCtxIter
        {
            Atom::StringFmtParseCtx ctx(fmtCtx);
            this->fmter.Parse(ctx);
            return fmtCtx.begin();
        }

        constexpr auto format(const T& in, Atom::_FmtFmtCtx& fmtCtx) -> Atom::_FmtFmtCtxOut
        {
            Atom::StringFmtCtx ctx(fmtCtx);
            this->fmter.Fmt(in, ctx);
            return fmtCtx.out();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// This contains actual implementation.
        /// ----------------------------------------------------------------------------------------
        Atom::StringFmtArgFmter<T> fmter;
    };
}
