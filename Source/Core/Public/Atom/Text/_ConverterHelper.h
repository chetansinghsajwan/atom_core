export module Atom.Core.Text:Encodings._ConverterHelper;

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// Requirements for {CharEncodingConverter} API.
    /// --------------------------------------------------------------------------------------------
    export template <typename TConverter, typename TInEncoding, typename TOutEncoding>
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
    export template <typename TInEncoding, typename TOutEncoding>
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
    template <typename TImpl, typename TInEncoding, typename TOutEncoding>
    class _CharEncodingConverterHelper
    {
    public:
        using TInChar = typename TInEncoding::TChar;
        using TOutChar = typename TOutEncoding::TChar;
        using TOutStr = Internal::Str<TOutEncoding>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInput, typename TOut>
        requires(RRangeOf<TInput, TInChar>)
            and (ROutput<TOut, TOutChar>)
        constexpr auto ConvertTo(const TInput& in, TOut out)
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
    template <typename TImpl, typename TEncoding>
    class _CharEncodingConverterHelper<TImpl, TEncoding, TEncoding>
    {
    public:
        using TChar = BasicChar<TCharEncoding>;
        using TStr = BasicStr<TCharEncoding>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Writes input to output as is.
        /// ----------------------------------------------------------------------------------------
        template <typename TInput, typename TOut>
        requires(RRangeOf<TInput, const TChar>)
            and (ROutput<TOut, TChar>)
        constexpr auto Convert(TInput&& in, TOut& out)
        {
            out += fwd(in);
        }
    };
}