#pragma once
#include "fmt/format.h"

#include "Atom/String.h"
#include "Atom/Containers.h"
#include "Atom/Exceptions.h"
#include "Atom/TTI.h"

namespace Atom::Fmt
{
	using _TFormatParseContext = ::fmt::basic_format_parse_context<Char>;
	using _TFormatParseContextIterator = typename _TFormatParseContext::iterator;

	using _TFormatContextOut = ::fmt::detail::buffer_appender<Char>;
	using _TFormatContext = ::fmt::basic_format_context<_TFormatContextOut, Char>;

	template <typename... TArgs>
	using _TFormatString = ::fmt::basic_format_string<Char, fmt::type_identity_t<TArgs>...>;
	using _TRuntimeFormatString = ::fmt::runtime_format_string<Char>;
	using _TStringView = ::fmt::basic_string_view<Char>;

	using _TFormatArg = ::fmt::basic_format_arg<_TFormatContext>;
	using _TFormatArgs = ::fmt::basic_format_args<_TFormatContext>;

	template <typename T>
	using _TFormatter = ::fmt::formatter<T, Char>;

	using _TFormatError = ::fmt::format_error;

	/// Exception thrown during formatting error.
	/// --------------------------------------------------------------------------------------------
	class FormatException: public Exception
	{
	public:
		FormatException(String msg) noexcept:
			Exception(msg.Data()) { }

		/// @TODO Fix this ugly code.
		/// ----------------------------------------------------------------------------------------
		FormatException(const _TFormatError& in_fmt_err) noexcept:
			Exception(TEXT("Not implemented.")) { }
		// FormatException(const _TFormatError& in_fmt_err) noexcept:
		// 	Exception(CharEncodingConverter<UTF8CharEncoding, CharEncoding>()
		// 		.Convert(UTF8StringView{ (const char8*)in_fmt_err.what() }.Iterator()).Data()) { }
	};

	/// A single format argument.
	/// --------------------------------------------------------------------------------------------
	class FormatArg
	{
	public:
		constexpr FormatArg(_TFormatArg in_fmt_arg) noexcept:
			m_fmt_arg{ in_fmt_arg } { }

	private:
		_TFormatArg m_fmt_arg;
	};

	/// List of format arguments.
	/// -------------------------------------------------------------------------------------------- 
	class FormatArgs
	{
	public:
		constexpr FormatArgs(_TFormatArgs in_fmt_args) noexcept:
			m_fmt_args{ in_fmt_args } { }

	public:
		constexpr FormatArg GetArg(int id) const
		{
			return FormatArg{ m_fmt_args.get(id) };
		}

		FormatArg GetArg(StringView name) const
		{
			_TStringView fmt_name = { name.Data(), name.Count() };
			return FormatArg{ m_fmt_args.get(fmt_name) };
		}

		int GetArgID(StringView name) const
		{
			_TStringView fmt_name = { name.Data(), name.Count() };
			return m_fmt_args.get_id(fmt_name);
		}

	private:
		_TFormatArgs m_fmt_args;
	};

	/// Context to parse format string.
	/// --------------------------------------------------------------------------------------------
	struct FormatParseContext
	{
		constexpr FormatParseContext(_TFormatParseContext& in_fmt_ctx) noexcept:
			fmt_ctx{ in_fmt_ctx } { }

		ArrayView<Char> GetRange() noexcept
		{
			return ArrayView<Char>{ fmt_ctx.begin(), fmt_ctx.end() };
		}

		void AdvanceTo(ArrayIterator<const Char> it) noexcept
		{
			fmt_ctx.advance_to(it.begin());
		}

		_TFormatParseContext& fmt_ctx;
	};

	/// Context to write formatted string.
	/// --------------------------------------------------------------------------------------------
	struct FormatContext
	{
		constexpr FormatContext(_TFormatContext& in_fmt_ctx) noexcept:
			fmt_ctx{ in_fmt_ctx } { }

		void Write(Char ch)
		{
			auto out = fmt_ctx.out();
			*out++ = ch;
			fmt_ctx.advance_to(MOVE(out));
		}

		void Write(RInputIterator<Char> auto chars)
		{
			auto out = fmt_ctx.out();
			for (Char ch : chars)
			{
				*out++ = ch;
			}

			fmt_ctx.advance_to(MOVE(out));
		}

		_TFormatContext& fmt_ctx;
	};

	/// Parses and writes string representation for types according to the specified format 
	/// specifiers.
	/// --------------------------------------------------------------------------------------------
	template <typename T>
	struct FormatterImpl;

	template <typename T>
	using Formatter = FormatterImpl<TTI::TRemoveCVRef<T>>;

	/// Ensures {TFormatter} is {Formatter} for type {T}.
	/// --------------------------------------------------------------------------------------------
	template <typename TFormatter, typename T>
	concept RFormatter = requires(TFormatter formatter)
	{
		formatter.Parse(declval(FormatParseContext&));
		formatter.Format(declval(T), declval(FormatContext&));
	};

	/// Formattable refers to a type for which exists a valid {Formatter<T>} specialization which 
	/// satisfies {RFormatter<Formatter<T>, T>} requirement.
	/// --------------------------------------------------------------------------------------------

	/// Ensures {T} is {Formattable}.
	/// --------------------------------------------------------------------------------------------
	template <typename T>
	concept RFormattable = RFormatter<Formatter<T>, T>;
}

namespace Atom::Fmt
{
	/// {Formatter} specialization for {StringView}.
	/// 
	/// @INTERNAL
	/// Uses {fmt::formatter<fmt::string_view>} specialization.
	/// -----------------------------------------------------------------------------------------------
	template < >
	struct FormatterImpl<StringView>
	{
		void Parse(FormatParseContext& ctx) noexcept
		{
			_TFormatParseContext& fmt_ctx = ctx.fmt_ctx;

			fmt_ctx.advance_to(fmt_formatter.parse(fmt_ctx));
		}

		void Format(StringView str, FormatContext& ctx) noexcept
		{
			_TFormatContext& fmt_ctx = ctx.fmt_ctx;

			_TStringView fmt_str{ str.Data(), str.Count() };
			fmt_ctx.advance_to(fmt_formatter.format(fmt_str, fmt_ctx));
		}

		_TFormatter<_TStringView> fmt_formatter;
	};

	static_assert(RFormattable<StringView>, "StringView is not formattable.");

	/// {Formatter} specialization for {Char} array.
	/// -----------------------------------------------------------------------------------------------
	template <usize N>
	struct FormatterImpl<Char[N]>: Formatter<StringView>
	{
		void Format(const Char(&chars)[N], FormatContext& ctx) noexcept
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
		constexpr void Format(const T& in, FormatContext& ctx) noexcept
		{
			Formatter<StringView>::Format(
				converter.Convert(in), ctx);
		}

		StringViewConverter<T> converter;
	};

	template <typename T>
	struct _TFormatterFilter
	{
		static constexpr bool Enable = true;
	};

	template <usize N>
	struct _TFormatterFilter<Char[N]>
	{
		static constexpr bool Enable = false;
	};
}

namespace fmt
{
	/// @INTERNAL
	/// 
	/// {fmt::formatter} specialization for all types that implement {Atom::Fmt::Formatter}.
	/// {fmt} uses this type and users specialize {Atom::Fmt::Formatter}.
	/// 
	/// This is specialized for {Char} character type only as {Atom} uses that type for 
	/// character representation.
	/// --------------------------------------------------------------------------------------------
	template <Atom::Fmt::RFormattable T>
	requires Atom::Fmt::_TFormatterFilter<T>::Enable
	struct formatter <T, Atom::Char>
	{
		Atom::Fmt::_TFormatParseContextIterator parse(Atom::Fmt::_TFormatParseContext& fmt_ctx)
		{
			Atom::Fmt::FormatParseContext ctx(fmt_ctx);
			this->formatter.Parse(ctx);
			return fmt_ctx.begin();
		}

		Atom::Fmt::_TFormatContextOut format(const T& in, Atom::Fmt::_TFormatContext& fmt_ctx)
		{
			Atom::Fmt::FormatContext ctx(fmt_ctx);
			this->formatter.Format(in, ctx);
			return fmt_ctx.out();
		}

		/// This contains actual implementation.
		/// ----------------------------------------------------------------------------------------
		Atom::Fmt::Formatter<T> formatter;
	};
}