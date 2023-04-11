#pragma once
#include "Atom/Core.h"
#include "Atom/Containers.h"

namespace Atom::Private
{
    /// --------------------------------------------------------------------------------------------
    /// This is used to specialize character encoding conversions, easily using partial 
    /// specialization. This is done for code reusability only.
    /// --------------------------------------------------------------------------------------------
    template <typename TFromCharEncoding, typename TToCharEncoding>
    struct CharEncodingConversionImpl
    {
        void ConvertChar();
    };
}

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Converts data from {TFromCharEncoding} character encoding to {TToCharEncoding}.
    /// 
    /// @TPARAM TFromCharEncoding Character encoding to convert data from.
    /// @TPARAM TToCharEncoding Character encoding to convert data to.
    /// --------------------------------------------------------------------------------------------
    template <typename TFromCharEncoding, typename TToCharEncoding>
    class CharEncodingConverter;

    /// --------------------------------------------------------------------------------------------
    /// Requirements for {CharEncodingConverter} API.
    /// --------------------------------------------------------------------------------------------
    template <typename TConverter, typename TFromCharEncoding, typename TToCharEncoding>
    concept RCharEncodingConverter = requires(TConverter converter,
        ConstIterableTestImpl<BasicChar<TFromCharEncoding>> in,
        BackInsertableTestImpl<BasicChar<TToCharEncoding>> out)
    {
        requires RDefaultConstructible<TConverter>;

        { converter.Convert(in) }
            -> RSameAs<BasicString<TToCharEncoding>>;

        { converter.Convert(in, out) }
            -> RSameAs<void>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {CharEncodingConverter<TFromCharEncoding, TToCharEncoding>} satisfies
    /// {RCharEncodingConverter<TFromCharEncoding, TToCharEncoding>}.
    /// --------------------------------------------------------------------------------------------
    template <typename TFromCharEncoding, typename TToCharEncoding>
    concept RCharEncodingConvertible = RCharEncodingConverter<
        CharEncodingConverter<TFromCharEncoding, TToCharEncoding>,
        TFromCharEncoding, TToCharEncoding>;

    /// --------------------------------------------------------------------------------------------
    /// Converts data from {TFromCharEncoding} character encoding to {TToCharEncoding} on demand.
    /// This doesn't process the whole string, only the requested part.
    /// --------------------------------------------------------------------------------------------
    template <typename TFromCharEncoding, typename TToCharEncoding, typename TInputIt>
    class CharEncodingLazyConverter;

    /// --------------------------------------------------------------------------------------------
    /// Requirements for {CharEncodingLazyConverter} API.
    /// --------------------------------------------------------------------------------------------
    template <typename TConverter, typename TFromCharEncoding, typename TToCharEncoding>
    concept RCharEncodingLazyConverter = requires(TConverter converter,
        ConstIterableTestImpl<BasicChar<TFromCharEncoding>> in)
    {
        requires RConstructible<TConverter, decltype(in), decltype(in)>;

        { converter.Next() }
            -> RSameAs<bool>;

        { converter.Get() }
            -> RSameAs<BasicChar<TToCharEncoding>>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {CharEncodingLazyConverter<TFromCharEncoding, TToCharEncoding>} satisfies
    /// {RCharEncodingLazyConverter<TFromCharEncoding, TToCharEncoding>}.
    /// --------------------------------------------------------------------------------------------
    template <typename TFromCharEncoding, typename TToCharEncoding>
    concept RCharEncodingLazyConvertible = RCharEncodingLazyConverter<
        CharEncodingLazyConverter<TFromCharEncoding, TToCharEncoding, ConstIterableTestImpl<BasicChar<TFromCharEncoding>>>,
        TFromCharEncoding, TToCharEncoding>;

    /// --------------------------------------------------------------------------------------------
    /// {CharEncodingConverter} specialization for same character encodings.
    /// --------------------------------------------------------------------------------------------
    template <typename TCharEncoding>
    class CharEncodingConverter<TCharEncoding, TCharEncoding>
    {
        using TChar = BasicChar<TCharEncoding>;
        using TString = BasicString<TCharEncoding>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Writes input to output as is.
        /// ----------------------------------------------------------------------------------------
        template <RConstIterable<TChar> TIn, RBackInsertable<TChar> TOut>
        constexpr void Convert(const TIn& in, TOut& out)
        {
            for (auto ch : in) out += ch;
        }

        /// ----------------------------------------------------------------------------------------
        /// Writes input to output as is.
        /// ----------------------------------------------------------------------------------------
        template <RConstIterable<TChar> TIn>
        constexpr TString Convert(const TIn& in)
        {
            TString out;
            Convert(in, out);

            return out;
        }

        template <RConstIterable<TChar> TIn>
        constexpr auto LazyConvert(const TIn& in)
        {
            using TLazyConverter = CharEncodingLazyConverter<
                TCharEncoding, TCharEncoding, decltype(TIn.begin())>;

            return TLazyConverter{ in.begin(), in.end() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {CharEncodingLazyConverter} specialization for same character encodings.
    /// --------------------------------------------------------------------------------------------
    template <typename TCharEncoding, typename TInputIt>
    class CharEncodingLazyConverter<TCharEncoding, TCharEncoding, TInputIt>
    {
        using TChar = BasicChar<TCharEncoding>;

    public:
        constexpr CharEncodingLazyConverter(TInputIt&& in_begin, TInputIt&& in_end) noexcept:
            _inputIt{ in_begin }, _inputEnd{ in_end } {}

        constexpr TChar Get() noexcept
        {
            return *_inputIt;
        }

        constexpr bool Next()
        {
            auto tmp = _inputIt++;
            return tmp != _inputEnd;
        }

    protected:
        TInputIt _inputIt;
        TInputIt _inputEnd;
    };

    /// --------------------------------------------------------------------------------------------
    /// {CharEncodingConverter} helper specialization for character encodings which provide
    /// implementation through {Private::CharEncodingConversionImpl<TFromCharEncoding, 
    /// TToCharEncoding>}.
    /// 
    /// @TODO Fix dependency in LazyConvert.
    /// --------------------------------------------------------------------------------------------
    template <typename TFromCharEncoding, typename TToCharEncoding>
    requires RConstructible<Private::CharEncodingConversionImpl<TFromCharEncoding, TToCharEncoding>>
    struct CharEncodingConverter<TFromCharEncoding, TToCharEncoding>
    {
        using TFromChar = typename TFromCharEncoding::TChar;
        using TToChar = typename TToCharEncoding::TChar;
        using TToString = BasicString<TToCharEncoding>;
        using TImpl = Private::CharEncodingConversionImpl<TFromCharEncoding, TToCharEncoding>;

        template <RConstIterable<TFromChar> TIn, RBackInsertable<TToChar> TOut>
        constexpr void Convert(const TIn& in, TOut& out)
        {
            auto end = in.end();
            for (auto it = in.begin(); it != end; it++)
            {
                TImpl::ConvertChar(it, end, out);
            }
        }

        template <RConstIterable<TFromChar> TIn>
        constexpr TToString Convert(const TIn& in)
        {
            TToString out;
            BasicStringWrapper<TToCharEncoding> outWrapper{ out };
            Convert(in, outWrapper);

            return out;
        }

        template <RConstIterable<TFromChar> TIn>
        constexpr auto LazyConvert(const TIn& in)
        {
            using TLazyConverter = CharEncodingLazyConverter<
                TFromCharEncoding, TToCharEncoding, decltype(TIn.begin())>;

            return TLazyConverter{ in.begin(), in.end() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {CharEncodingLazyConverter} helper specialization for character encodings which provide
    /// implementation through {Private::CharEncodingConversionImpl<TFromCharEncoding, 
    /// TToCharEncoding>}.
    /// 
    /// @TODO Fix output buffer size.
    /// --------------------------------------------------------------------------------------------
    template <typename TFromCharEncoding, typename TToCharEncoding, typename TInputIt>
    requires RConstructible<Private::CharEncodingConversionImpl<TFromCharEncoding, TToCharEncoding>>
    struct CharEncodingLazyConverter<TFromCharEncoding, TToCharEncoding, TInputIt>
    {
        using TThis = CharEncodingLazyConverter;
        using TFromChar = typename TFromCharEncoding::TChar;
        using TToChar = typename TToCharEncoding::TChar;
        using TImpl = Private::CharEncodingConversionImpl<TFromCharEncoding, TToCharEncoding>;

    public:
        constexpr CharEncodingLazyConverter(TInputIt&& in_begin, TInputIt&& in_end) noexcept:
            _inputIt{ in_begin }, _inputEnd{ in_end },
            _out{ 0 }, _outIndex{ -1 }
        {
            _ProcessNextChar();
        }

        constexpr TToChar Get() noexcept
        {
            return _out[_outIndex];
        }

        constexpr bool Next()
        {
            if (_outIndex == -1)
            {
                _ProcessNextChar();
            }

            _outIndex--;
            return _inputIt != _inputEnd;
        }

    protected:
        void _ProcessNextChar()
        {
            struct Output
            {
                constexpr void operator += (TToChar ch) noexcept
                {
                    out[outIndex++] = ch;
                }

                TToChar* out;
                uint8& outIndex;
            };

            TImpl::Convert(_inputIt, _inputEnd, Output{ _out, _outIndex });
        }

    protected:
        TToChar _out[4];
        uint8 _outIndex;

        TInputIt _inputIt;
        TInputIt _inputEnd;
    };
};