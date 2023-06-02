#pragma once
#include "String.h"
#include "StringViewConverter.h"
#include "StringConverter.h"

#include "Atom/Exceptions.h"
#include "Atom/TTI.h"

#include "fmt/format.h"

namespace Atom
{
	using _FmtFormatParseContext = ::fmt::basic_format_parse_context<Char>;
	using _FmtFormatParseContextIterator = typename _FmtFormatParseContext::iterator;

	using _FmtFormatContextOut = ::fmt::detail::buffer_appender<Char>;
	using _FmtFormatContext = ::fmt::basic_format_context<_FmtFormatContextOut, Char>;

	template <typename... TArgs>
	using _FmtFormatString = ::fmt::basic_format_string<Char, fmt::type_identity_t<TArgs>...>;
	using _FmtRuntimeFormatString = ::fmt::runtime_format_string<Char>;
	using _FmtStringView = ::fmt::basic_string_view<Char>;

	using _FmtFormatArg = ::fmt::basic_format_arg<_FmtFormatContext>;
	using _FmtFormatArgs = ::fmt::basic_format_args<_FmtFormatContext>;

	template <typename T>
	using _FmtFormatter = ::fmt::formatter<T, Char>;

	using _FmtFormatError = ::fmt::format_error;

	/// --------------------------------------------------------------------------------------------
	/// Exception thrown during formatting error.
	/// --------------------------------------------------------------------------------------------
	class StringFormatException: public Exception
	{
	public:
		StringFormatException(String msg) noexcept:
			Exception(msg.Data()) { }

		/// @TODO Fix this ugly code.
		/// ----------------------------------------------------------------------------------------
		StringFormatException(const _FmtFormatError& in_fmt_err) noexcept:
			Exception(TEXT("Not implemented.")) { }
		// StringFormatException(const _FmtFormatError& in_fmt_err) noexcept:
		// 	Exception(CharEncodingConverter<UTF8CharEncoding, CharEncoding>()
		// 		.Convert(UTF8StringView{ (const char8*)in_fmt_err.what() }.Iterator()).Data()) { }
	};

	/// --------------------------------------------------------------------------------------------
	/// A single format argument.
	/// --------------------------------------------------------------------------------------------
	class StringFormatArg
	{
	public:
		constexpr StringFormatArg(_FmtFormatArg in_fmt_arg) noexcept:
			m_fmt_arg{ in_fmt_arg } { }

	private:
		_FmtFormatArg m_fmt_arg;
	};

	/// --------------------------------------------------------------------------------------------
	/// List of format arguments.
	/// -------------------------------------------------------------------------------------------- 
	class StringFormatArgs
	{
	public:
		constexpr StringFormatArgs(_FmtFormatArgs in_fmt_args) noexcept:
			m_fmt_args{ in_fmt_args } { }

	public:
		constexpr StringFormatArg GetArg(int id) const
		{
			return StringFormatArg{ m_fmt_args.get(id) };
		}

		StringFormatArg GetArg(StringView name) const
		{
			_FmtStringView fmt_name = { name.Data(), name.Count() };
			return StringFormatArg{ m_fmt_args.get(fmt_name) };
		}

		int GetArgID(StringView name) const
		{
			_FmtStringView fmt_name = { name.Data(), name.Count() };
			return m_fmt_args.get_id(fmt_name);
		}

	private:
		_FmtFormatArgs m_fmt_args;
	};

	/// --------------------------------------------------------------------------------------------
	/// Context to parse format string.
	/// --------------------------------------------------------------------------------------------
	struct StringFormatParseContext
	{
		constexpr StringFormatParseContext(_FmtFormatParseContext& in_fmt_ctx) noexcept:
			fmt_ctx{ in_fmt_ctx } { }

		ArrayView<Char> GetRange() noexcept
		{
			return ArrayView<Char>{ fmt_ctx.begin(), fmt_ctx.end() };
		}

		void AdvanceTo(ArrayIterator<const Char> it) noexcept
		{
			fmt_ctx.advance_to(&*it);
		}

		_FmtFormatParseContext& fmt_ctx;
	};

	/// --------------------------------------------------------------------------------------------
	/// Context to write formatted string.
	/// --------------------------------------------------------------------------------------------
	struct StringFormatContext
	{
		constexpr StringFormatContext(_FmtFormatContext& in_fmt_ctx) noexcept:
			fmt_ctx{ in_fmt_ctx } { }

		void Write(Char ch)
		{
			auto out = fmt_ctx.out();
			*out++ = ch;
			fmt_ctx.advance_to(MOVE(out));
		}

		template <typename TRange>
		requires RFwdRange<TRange, const Char>
		void Write(const TRange& chars)
		{
			auto out = fmt_ctx.out();
			for (Char ch : chars)
			{
				*out++ = ch;
			}

			fmt_ctx.advance_to(MOVE(out));
		}

		_FmtFormatContext& fmt_ctx;
	};

	/// --------------------------------------------------------------------------------------------
	/// Parses and writes string representation for types according to the specified format 
	/// specifiers.
	/// --------------------------------------------------------------------------------------------
	template <typename T>
	struct FormatterImpl;

	/// --------------------------------------------------------------------------------------------
	/// 
	/// --------------------------------------------------------------------------------------------
	template <typename T>
	using Formatter = FormatterImpl<TTI::TRemoveCVRef<T>>;

	/// --------------------------------------------------------------------------------------------
	/// Ensures {TFormatter} is {Formatter} for type {T}.
	/// --------------------------------------------------------------------------------------------
	template <typename TFormatter, typename T>
	concept RStringArgFormatter = requires(TFormatter formatter)
	{
		formatter.Parse(declval(StringFormatParseContext&));
		formatter.Format(declval(T), declval(StringFormatContext&));
	};

	/// --------------------------------------------------------------------------------------------
	/// StringArgFormattable refers to a type for which exists a valid {Formatter<T>} specialization which 
	/// satisfies {RStringArgFormatter<Formatter<T>, T>} requirement.
	/// --------------------------------------------------------------------------------------------

	/// --------------------------------------------------------------------------------------------
	/// Ensures {T} is {StringArgFormattable}.
	/// --------------------------------------------------------------------------------------------
	template <typename T>
	concept RStringArgFormattable = RStringArgFormatter<Formatter<T>, T>;
}

namespace Atom
{
	/// {Formatter} specialization for {StringView}.
	/// 
	/// @INTERNAL
	/// Uses {fmt::formatter<fmt::string_view>} specialization.
	/// -----------------------------------------------------------------------------------------------
	template < >
	struct FormatterImpl<StringView>
	{
		void Parse(StringFormatParseContext& ctx) noexcept
		{
			_FmtFormatParseContext& fmt_ctx = ctx.fmt_ctx;

			fmt_ctx.advance_to(fmt_formatter.parse(fmt_ctx));
		}

		void Format(StringView str, StringFormatContext& ctx) noexcept
		{
			_FmtFormatContext& fmt_ctx = ctx.fmt_ctx;

			_FmtStringView fmt_str{ str.Data(), str.Count() };
			fmt_ctx.advance_to(fmt_formatter.format(fmt_str, fmt_ctx));
		}

		_FmtFormatter<_FmtStringView> fmt_formatter;
	};

	static_assert(RStringArgFormattable<StringView>, "StringView is not formattable.");

	/// {Formatter} specialization for {Char} array.
	/// -----------------------------------------------------------------------------------------------
	template <usize N>
	struct FormatterImpl<Char[N]>: Formatter<StringView>
	{
		void Format(const Char(&chars)[N], StringFormatContext& ctx) noexcept
		{
			StringView str{ chars, N };
			Formatter<StringView>::Format(str, ctx);
		}
	};

	/// {Formatter} specialization for types which satisfy {RStringViewConvertible} requirement.
	/// -----------------------------------------------------------------------------------------------
	template <RStringViewConvertible T>
	struct FormatterImpl<T>: Formatter<StringView>
	{
		constexpr void Format(const T& in, StringFormatContext& ctx) noexcept
		{
			Formatter<StringView>::Format(
				converter.Convert(in), ctx);
		}

		StringViewConverter<T> converter;
	};

	template <typename T>
	struct _FmtFormatterFilter
	{
		static constexpr bool Enable = true;
	};

	template <usize N>
	struct _FmtFormatterFilter<Char[N]>
	{
		static constexpr bool Enable = false;
	};
}

namespace fmt
{
	/// --------------------------------------------------------------------------------------------
	/// @INTERNAL
	/// 
	/// {fmt::formatter} specialization for all types that implement {Atom::Formatter}.
	/// {fmt} uses this type and users specialize {Atom::Formatter}.
	/// 
	/// This is specialized for {Char} character type only as {Atom} uses that type for 
	/// character representation.
	/// --------------------------------------------------------------------------------------------
	template <Atom::RStringArgFormattable T>
	requires Atom::_FmtFormatterFilter<T>::Enable
	struct formatter <T, Atom::Char>
	{
		Atom::_FmtFormatParseContextIterator parse(Atom::_FmtFormatParseContext& fmt_ctx)
		{
			Atom::StringFormatParseContext ctx(fmt_ctx);
			this->formatter.Parse(ctx);
			return fmt_ctx.begin();
		}

		Atom::_FmtFormatContextOut format(const T& in, Atom::_FmtFormatContext& fmt_ctx)
		{
			Atom::StringFormatContext ctx(fmt_ctx);
			this->formatter.Format(in, ctx);
			return fmt_ctx.out();
		}

		/// ----------------------------------------------------------------------------------------
		/// This contains actual implementation.
		/// ----------------------------------------------------------------------------------------
		Atom::Formatter<T> formatter;
	};
}