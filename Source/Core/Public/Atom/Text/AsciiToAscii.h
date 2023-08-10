export module Atom.Core.Text:Encodings.AsciiToAscii;

namespace Atom
{
    template < >
    class _AsciiToAsciiConversionImpl
    {
    public:
        template <typename TIn, typename TOut>
        constexpr void ConvertChar(const TIn& in, TOut& out)
        {
        }
    };

    using AsciiToAsciiConverter = _CharEncodingConverterHelper<
        _AsciiToAsciiConversionImpl, AsciiEncoding, AsciiEncoding>;

    template <typename TInput>
    using AsciiToAsciiLazyConverter = _CharEncodingLazyConverterHelper<
        _AsciiToAsciiConversionImpl, AsciiEncoding, AsciiEncoding, TInput>;
}