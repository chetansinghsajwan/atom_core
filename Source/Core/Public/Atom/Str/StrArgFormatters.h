#pragma once
#include "fmt/format.h"

#include "Str.h"
#include "StrViewConverter.h"
#include "StrConverter.h"

#include "Atom/Exceptions.h"
#include "Atom/TTI.h"

namespace Atom
{
	using _FmtFmtParseCtx = ::fmt::basic_format_parse_context<Char>;
	using _FmtFmtParseCtxIter = tname _FmtFmtParseCtx::iterator;

	using _FmtFmtCtxOut = ::fmt::detail::buffer_appender<Char>;
	using _FmtFmtCtx = ::fmt::basic_format_context<_FmtFmtCtxOut, Char>;

	template <tname... TArgs>
	using _FmtFmtStr = ::fmt::basic_format_string<Char, fmt::type_identity_t<TArgs>...>;
	using _FmtRunFmtStr = ::fmt::runtime_format_string<Char>;
	using _FmtStrView = ::fmt::basic_string_view<Char>;

	template <tname T>
	using _FmtFmter = ::fmt::formatter<T, Char>;

	using _FmtFmtEx = ::fmt::format_error;

	/// --------------------------------------------------------------------------------------------
	/// Exception thrown during formatting error.
	/// --------------------------------------------------------------------------------------------
	class StrFmtEx: public Exception
	{
	public:
		ctor StrFmtEx(Str msg) noex:
			Exception(MOVE(msg)) { }

		/// @TODO Fix this ugly code.
		/// ----------------------------------------------------------------------------------------
		ctor StrFmtEx(const _FmtFmtEx& fmtEx) noex:
			Exception(TEXT("Not implemented.")) { }
		// StrFmtEx(const _FmtFmtEx& fmtEx) noex:
		// 	Exception(CharEncodingConverter<UTF8CharEncoding, CharEncoding>()
		// 		.Convert(UTF8StrView{ (const char8*)fmtEx.what() }.Iter()).Data()) { }
	};

	/// --------------------------------------------------------------------------------------------
	/// Context to parse format string.
	/// --------------------------------------------------------------------------------------------
	struct StrFmtParseCtx
	{
		constexpr ctor StrFmtParseCtx(_FmtFmtParseCtx& fmtCtx) noex:
			_fmtCtx{ fmtCtx } { }

		fn GetRange() noex -> StrView
		{
			return StrView{ Range(_fmtCtx.begin(), _fmtCtx.end()) };
		}

		fn AdvanceTo(ArrIter<Char> it) noex
		{
			_fmtCtx.advance_to(&*it);
		}

		_FmtFmtParseCtx& _fmtCtx;
	};

	/// --------------------------------------------------------------------------------------------
	/// Context to write formatted string.
	/// --------------------------------------------------------------------------------------------
	struct StrFmtCtx
	{
		constexpr ctor StrFmtCtx(_FmtFmtCtx& fmtCtx) noex:
			_fmtCtx{ fmtCtx } { }

		fn Write(Char ch)
		{
			auto out = _fmtCtx.out();
			*out++ = ch;
			_fmtCtx.advance_to(MOVE(out));
		}

		template <tname TRange>
		requires RRangeOf<TRange, Char>
		fn Write(const TRange& chars)
		{
			auto out = _fmtCtx.out();
			for (Char ch : chars)
			{
				*out++ = ch;
			}

			_fmtCtx.advance_to(MOVE(out));
		}

		_FmtFmtCtx& _fmtCtx;
	};

	/// --------------------------------------------------------------------------------------------
	/// Parses and writes string representation for types according to the specified format 
	/// specifiers.
	/// --------------------------------------------------------------------------------------------
	template <tname T>
	struct StrFmtArgFmterImpl;

	/// --------------------------------------------------------------------------------------------
	/// 
	/// --------------------------------------------------------------------------------------------
	template <tname T>
	using StrFmtArgFmter = StrFmtArgFmterImpl<TTI::TRemoveCVRef<T>>;

	/// --------------------------------------------------------------------------------------------
	/// Ensures {TFmter} is {StrFmtArgFmter} for type {T}.
	/// --------------------------------------------------------------------------------------------
	template <tname TFmter, tname T>
	concept RStrFmtArgFmter = requires(TFmter fmter)
	{
		fmter.Parse(declval(StrFmtParseCtx&));
		fmter.Fmt(declval(T), declval(StrFmtCtx&));
	};

	/// --------------------------------------------------------------------------------------------
	/// StrArgFmtable refers to a type for which exists a valid {StrFmtArgFmter<T>} specialization 
	/// which satisfies {RStrFmtArgFmter<StrFmtArgFmter<T>, T>} requirement.
	/// --------------------------------------------------------------------------------------------

	/// --------------------------------------------------------------------------------------------
	/// Ensures {T} is {StrArgFmtable}.
	/// --------------------------------------------------------------------------------------------
	template <tname T>
	concept RStrFmtArgFmtable = RStrFmtArgFmter<StrFmtArgFmter<T>, T>;
}

namespace Atom
{
	/// -----------------------------------------------------------------------------------------------
	/// {StrFmtArgFmter} specialization for {StrView}.
	/// 
	/// @INTERNAL
	/// Uses {fmt::fmter<fmt::string_view>} specialization.
	/// -----------------------------------------------------------------------------------------------
	template < >
	struct StrFmtArgFmterImpl<StrView>
	{
		fn Parse(StrFmtParseCtx& ctx) noex
		{
			_FmtFmtParseCtx& fmtCtx = ctx._fmtCtx;

			fmtCtx.advance_to(_fmtFmter.parse(fmtCtx));
		}

		fn Fmt(StrView str, StrFmtCtx& ctx) noex
		{
			_FmtFmtCtx& fmtCtx = ctx._fmtCtx;

			_FmtStrView fmt_str{ str.Data(), str.Count() };
			fmtCtx.advance_to(_fmtFmter.format(fmt_str, fmtCtx));
		}

		_FmtFmter<_FmtStrView> _fmtFmter;
	};

	static_assert(RStrFmtArgFmtable<StrView>, "StrView is not formattable.");

	/// -----------------------------------------------------------------------------------------------
	/// {StrFmtArgFmter} specialization for {Char} arr.
	/// -----------------------------------------------------------------------------------------------
	template <usize N>
	struct StrFmtArgFmterImpl<Char[N]>: StrFmtArgFmter<StrView>
	{
		fn Fmt(const Char(&chars)[N], StrFmtCtx& ctx) noex
		{
			StrView str{ chars, N };
			StrFmtArgFmter<StrView>::Fmt(str, ctx);
		}
	};

	/// -----------------------------------------------------------------------------------------------
	/// {StrFmtArgFmter} specialization for types which satisfy {RStrViewConvertible} requirement.
	/// -----------------------------------------------------------------------------------------------
	template <RStrViewConvertible T>
	struct StrFmtArgFmterImpl<T>: StrFmtArgFmter<StrView>
	{
		constexpr fn Fmt(const T& in, StrFmtCtx& ctx) noex
		{
			StrFmtArgFmter<StrView>::Fmt(
				convter.Convert(in), ctx);
		}

		StrViewConverter<T> convter;
	};

	template <tname T>
	struct _FmtFmtfierFilter
	{
		static constexpr bool Enable = true;
	};

	template <usize N>
	struct _FmtFmtfierFilter<Char[N]>
	{
		static constexpr bool Enable = false;
	};
}

namespace fmt
{
	/// --------------------------------------------------------------------------------------------
	/// @INTERNAL
	/// 
	/// {fmt::fmter} specialization for all types that implement {Atom::StrFmtArgFmter}.
	/// {fmt} uses this type and users specialize {Atom::StrFmtArgFmter}.
	/// 
	/// This is specialized for {Char} character type only as {Atom} uses that type for 
	/// character representation.
	/// --------------------------------------------------------------------------------------------
	template <Atom::RStrFmtArgFmtable T>
	requires Atom::_FmtFmtfierFilter<T>::Enable
	struct formatter <T, Atom::Char>
	{
		Atom::_FmtFmtParseCtxIter parse(Atom::_FmtFmtParseCtx& fmtCtx)
		{
			Atom::StrFmtParseCtx ctx(fmtCtx);
			this->fmter.Parse(ctx);
			return fmtCtx.begin();
		}

		Atom::_FmtFmtCtxOut format(const T& in, Atom::_FmtFmtCtx& fmtCtx)
		{
			Atom::StrFmtCtx ctx(fmtCtx);
			this->fmter.Fmt(in, ctx);
			return fmtCtx.out();
		}

		/// ----------------------------------------------------------------------------------------
		/// This contains actual implementation.
		/// ----------------------------------------------------------------------------------------
		Atom::StrFmtArgFmter<T> fmter;
	};
}