#pragma once
#include "Atom/Core.h"
#include "Atom/Containers.h"

#include "Atom/String/BasicChar.h"
#include "Atom/String/BasicString.h"

namespace Atom::Private
{
    /// --------------------------------------------------------------------------------------------
    /// This is used to specialize character encoding conversions, easily using partial 
    /// specialization. This is done for code reusability only.
    /// --------------------------------------------------------------------------------------------
    template <typename TFromCharEncoding, typename TToCharEncoding>
    struct CharEncodingConversionImpl
    {
        CharEncodingConversionImpl() = delete;

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
        Internal::FwdItMock<BasicChar<TFromCharEncoding>> in,
        Internal::OutputWriterMock<BasicChar<TToCharEncoding>> out)
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
    template <typename TFromCharEncoding, typename TToCharEncoding, typename TInput>
    class CharEncodingLazyConverter;

    /// --------------------------------------------------------------------------------------------
    /// Requirements for {CharEncodingLazyConverter} API.
    /// --------------------------------------------------------------------------------------------
    template <typename TConverter, typename TFromCharEncoding, typename TToCharEncoding>
    concept RCharEncodingLazyConverter = requires(TConverter converter,
        Internal::FwdItMock<BasicChar<TFromCharEncoding>> in)
    {
        requires RConstructible<TConverter, decltype(in)>;

        { converter.Next() }
            -> RSameAs<bool>;

        { converter.Get() }
            -> RConvertibleTo<BasicChar<TToCharEncoding>>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {CharEncodingLazyConverter<TFromCharEncoding, TToCharEncoding>} satisfies
    /// {RCharEncodingLazyConverter<TFromCharEncoding, TToCharEncoding>}.
    /// --------------------------------------------------------------------------------------------
    template <typename TFromCharEncoding, typename TToCharEncoding>
    concept RCharEncodingLazyConvertible = RCharEncodingLazyConverter<
        CharEncodingLazyConverter<TFromCharEncoding, TToCharEncoding, Internal::FwdItMock<BasicChar<TFromCharEncoding>>>,
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
        template <RFwdIt<TChar> TInput, ROutput<TChar> TOut>
        constexpr void Convert(TInput in, TOut out)
        {
            for (auto ch : in) out += ch;
        }

        /// ----------------------------------------------------------------------------------------
        /// Writes input to output as is.
        /// ----------------------------------------------------------------------------------------
        template <RFwdIt<TChar> TInput>
        constexpr TString Convert(TInput in)
        {
            TString out;
            Convert(in, out);

            return out;
        }

        template <RFwdIt<TChar> TInput>
        constexpr auto LazyConvert(TInput in)
        {
            using TLazyConverter = CharEncodingLazyConverter<
                TCharEncoding, TCharEncoding, decltype(TInput.begin())>;

            return TLazyConverter{ in.begin(), in.end() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {CharEncodingLazyConverter} specialization for same character encodings.
    /// --------------------------------------------------------------------------------------------
    template <typename TCharEncoding, typename TInput>
    class CharEncodingLazyConverter<TCharEncoding, TCharEncoding, TInput>
    {
        using TChar = BasicChar<TCharEncoding>;

    public:
        constexpr CharEncodingLazyConverter(TInput&& input) noexcept:
            _input{ input } { }

    public:
        constexpr TChar Get() noexcept
        {
            return _input.Get();
        }

        constexpr bool Next()
        {
            return _input.Next();
        }

        constexpr bool HasNext() const noexcept
        {
            return _input.HasNext();
        }

    protected:
        TInput _input;
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

        template <RFwdIt<TFromChar> TInput, ROutput<TToChar> TOut>
        constexpr void Convert(TInput in, TOut out)
        {
            auto end = in.end();
            for (auto it = in.begin(); it != end; it++)
            {
                TImpl::ConvertChar(it, out);
            }
        }

        template <RFwdIt<TFromChar> TInput>
        constexpr TToString Convert(TInput in)
        {
            TToString out;
            Convert(in, out);

            return out;
        }

        template <RFwdIt<TFromChar> TInput>
        constexpr auto LazyConvert(TInput in)
        {
            using TLazyConverter = CharEncodingLazyConverter<
                TFromCharEncoding, TToCharEncoding, TInput>;

            return TLazyConverter{ in };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {CharEncodingLazyConverter} helper specialization for character encodings which provide
    /// implementation through {Private::CharEncodingConversionImpl<TFromCharEncoding, 
    /// TToCharEncoding>}.
    /// 
    /// @TODO Fix output buffer size.
    /// --------------------------------------------------------------------------------------------
    template <typename TFromCharEncoding, typename TToCharEncoding, RFwdIt<
        BasicChar<TFromCharEncoding>> TInput>
    requires RConstructible<Private::CharEncodingConversionImpl<TFromCharEncoding, TToCharEncoding>>
    struct CharEncodingLazyConverter<TFromCharEncoding, TToCharEncoding, TInput>
    {
        using TThis = CharEncodingLazyConverter;
        using TFromChar = typename TFromCharEncoding::TChar;
        using TToChar = typename TToCharEncoding::TChar;
        using TImpl = Private::CharEncodingConversionImpl<TFromCharEncoding, TToCharEncoding>;

    public:
        constexpr CharEncodingLazyConverter(TInput&& input) noexcept:
            _input{ fwd(input) }, _out{ 0 }, _outIndex{ -1 }
        {
            _ProcessNextChar();
        }

        constexpr TToChar& Get() noexcept
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
            return _input.HasNext();
        }

        constexpr bool HasNext() const noexcept
        {
            return _outIndex > 0 || _input.HasNext();
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

            TImpl::ConvertChar(_input, Output{ _out, _outIndex });
        }

    protected:
        TToChar _out[4];
        uint8 _outIndex;

        TInput _input;
    };
};