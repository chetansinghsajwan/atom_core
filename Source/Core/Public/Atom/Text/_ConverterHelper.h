export module Atom.Core.Text:Encodings._ConverterHelper;

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// Requirements for {CharEncodingConverter} API.
    /// --------------------------------------------------------------------------------------------
    export template <tname TConverter, tname TInEncoding, tname TOutEncoding>
    concept RCharEncodingConverter = requires(TConverter converter,
        Internal::RangeMock<BasicChar<TInEncoding>> in,
        Internal::OutputMock<BasicChar<TOutEncoding>> out)
    {
        requires RDefaultConstructible<TConverter>;

        { converter.Convert(in) }
            -> RSameAs<BasicStr<TOutEncoding>>;

        { converter.Convert(in, out) }
            -> RSameAs<void>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {CharEncodingConverter<TInEncoding, TOutEncoding>} satisfies
    /// {RCharEncodingConverter<TInEncoding, TOutEncoding>}.
    /// --------------------------------------------------------------------------------------------
    export template <tname TInEncoding, tname TOutEncoding>
    concept RCharEncodingConvertible = RCharEncodingConverter<
        CharEncodingConverter<TInEncoding, TOutEncoding>,
        TInEncoding, TOutEncoding>;
}

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// Converts data from {TInEncoding} character encoding to {TOutEncoding}.
    /// 
    /// @TPARAM TImpl Conversion impl.
    /// @TPARAM TInEncoding Character encoding to convert data from.
    /// @TPARAM TOutEncoding Character encoding to convert data to.
    /// --------------------------------------------------------------------------------------------
    template <tname TImpl, tname TInEncoding, tname TOutEncoding>
    struct _CharEncodingConverterHelper
    {
        using TInChar = tname TInEncoding::TChar;
        using TOutChar = tname TOutEncoding::TChar;
        using TOutStr = Internal::Str<TOutEncoding>;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <tname TInput, tname TOut>
        requires RRangeOf<TInput, TInChar> && ROutput<TOut, TOutChar>
        constexpr void ConvertTo(const TInput& in, TOut out)
        {
            auto end = in.end();
            for (auto it = in.begin(); it != end; it++)
            {
                TImpl::ConvertChar(it, out);
            }
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Specialization for same {CharEncoding}.
    /// --------------------------------------------------------------------------------------------
    template <tname TImpl, tname TEncoding>
    struct _CharEncodingConverterHelper<TImpl, TEncoding, TEncoding>
    {
        using TChar = BasicChar<TCharEncoding>;
        using TStr = BasicStr<TCharEncoding>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Writes input to output as is.
        /// ----------------------------------------------------------------------------------------
        template <tname TInput, tname TOut>
        requires RRangeOf<TInput, const TChar> && ROutput<TOut, TChar>
        constexpr void Convert(TInput&& in, TOut& out)
        {
            out += FORWARD(in);
        }
    };
}