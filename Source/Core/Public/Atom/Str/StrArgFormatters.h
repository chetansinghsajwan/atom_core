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
	class StrFmtEx extends Exception
	{
		pub ctor StrFmtEx(Str msg) noex:
			Exception(mov(msg)) { }

		/// ----------------------------------------------------------------------------------------
		/// @TODO Fix this ugly code.
		/// ----------------------------------------------------------------------------------------
		pub ctor StrFmtEx(const _FmtFmtEx& fmtEx) noex:
			Exception("Not implemented.") { }
		// StrFmtEx(const _FmtFmtEx& fmtEx) noex:
		// 	Exception(CharEncodingConverter<UTF8CharEncoding, CharEncoding>()
		// 		.Convert(UTF8StrView{ (const char8*)fmtEx.what() }.Iter()).Data()) { }
	};

	/// --------------------------------------------------------------------------------------------
	/// Context to parse format string.
	/// --------------------------------------------------------------------------------------------
	class StrFmtParseCtx
	{
		pub cexpr ctor StrFmtParseCtx(_FmtFmtParseCtx& fmtCtx) noex:
			_fmtCtx{ fmtCtx } { }

		pub fn GetRange() noex -> StrView
		{
			auto range = Range(_fmtCtx.begin(), _fmtCtx.end());
			return StrView(range);
		}

		pub fn AdvanceTo(ArrIter<Char> it) noex
		{
			_fmtCtx.advance_to(&*it);
		}

		pub _FmtFmtParseCtx& _fmtCtx;
	};

	/// --------------------------------------------------------------------------------------------
	/// Context to write formatted string.
	/// --------------------------------------------------------------------------------------------
	class StrFmtCtx
	{
		pub cexpr ctor StrFmtCtx(_FmtFmtCtx& fmtCtx) noex:
			_fmtCtx{ fmtCtx } { }

		pub fn Write(Char ch)
		{
			auto out = _fmtCtx.out();
			*out++ = ch;
			_fmtCtx.advance_to(mov(out));
		}

		pub template <tname TRange>
		requires RRangeOf<TRange, Char>
		fn Write(const TRange& chars)
		{
			auto out = _fmtCtx.out();
			for (Char ch : chars)
			{
				*out++ = ch;
			}

			_fmtCtx.advance_to(mov(out));
		}

		pub _FmtFmtCtx& _fmtCtx;
	};

	/// --------------------------------------------------------------------------------------------
	/// Parses and writes string representation for types according to the specified format 
	/// specifiers.
	/// --------------------------------------------------------------------------------------------
	template <tname T>
	class StrFmtArgFmterImpl;

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
	/// --------------------------------------------------------------------------------------------
	/// {StrFmtArgFmter} specialization for {StrView}.
	/// 
	/// @INTERNAL
	/// Uses {fmt::fmter<fmt::string_view>} specialization.
	/// --------------------------------------------------------------------------------------------
	template < >
	class StrFmtArgFmterImpl<StrView>
	{
		pub fn Parse(StrFmtParseCtx& ctx) noex
		{
			_FmtFmtParseCtx& fmtCtx = ctx._fmtCtx;

			fmtCtx.advance_to(_fmtFmter.parse(fmtCtx));
		}

		pub fn Fmt(StrView str, StrFmtCtx& ctx) noex
		{
			_FmtFmtCtx& fmtCtx = ctx._fmtCtx;

			_FmtStrView fmt_str{ str.Data(), str.Count() };
			fmtCtx.advance_to(_fmtFmter.format(fmt_str, fmtCtx));
		}

		pub _FmtFmter<_FmtStrView> _fmtFmter;
	};

	static_assert(RStrFmtArgFmtable<StrView>, "StrView is not formattable.");

	/// --------------------------------------------------------------------------------------------
	/// {StrFmtArgFmter} specialization for {Char} arr.
	/// --------------------------------------------------------------------------------------------
	template <usize N>
	class StrFmtArgFmterImpl<Char[N]> extends StrFmtArgFmter<StrView>
	{
		pub fn Fmt(const Char(&chars)[N], StrFmtCtx& ctx) noex
		{
			StrView str{ chars, N };
			StrFmtArgFmter<StrView>::Fmt(str, ctx);
		}
	};

	/// --------------------------------------------------------------------------------------------
	/// {StrFmtArgFmter} specialization for types which satisfy {RStrViewConvertible} requirement.
	/// --------------------------------------------------------------------------------------------
	template <tname T>
	requires RStrViewConvertible<T>
	class StrFmtArgFmterImpl<T> extends StrFmtArgFmter<StrView>
	{
		pub cexpr fn Fmt(const T& in, StrFmtCtx& ctx) noex
		{
			StrFmtArgFmter<StrView>::Fmt(
				convter.Convert(in), ctx);
		}

		pub StrViewConverter<T> convter;
	};

	template <tname T>
	class _FmtFmtfierFilter
	{
		pub static cexpr bool Enable = true;
	};

	template <usize N>
	class _FmtFmtfierFilter<Char[N]>
	{
		pub static cexpr bool Enable = false;
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
	class formatter <T, Atom::Char>
	{
		pub fn parse(Atom::_FmtFmtParseCtx& fmtCtx) -> Atom::_FmtFmtParseCtxIter
		{
			Atom::StrFmtParseCtx ctx(fmtCtx);
			self.fmter.Parse(ctx);
			return fmtCtx.begin();
		}

		pub fn format(const T& in, Atom::_FmtFmtCtx& fmtCtx) -> Atom::_FmtFmtCtxOut
		{
			Atom::StrFmtCtx ctx(fmtCtx);
			self.fmter.Fmt(in, ctx);
			return fmtCtx.out();
		}

		/// ----------------------------------------------------------------------------------------
		/// This contains actual implementation.
		/// ----------------------------------------------------------------------------------------
		pub Atom::StrFmtArgFmter<T> fmter;
	};
}