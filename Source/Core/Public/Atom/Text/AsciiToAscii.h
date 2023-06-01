export module Atom.Core.Text:Encodings.AsciiToAscii;

namespace Atom
{
    template < >
    struct _AsciiToAsciiConversionImpl
    {
        template <typename TIn, typename TOut>
        constexpr void ConvertChar(const TIn& in, TOut& out)
        {
        }
    };

    export using AsciiToAsciiConverter = _CharEncodingConverterHelper<
        _AsciiToAsciiConversionImpl, AsciiEncoding, AsciiEncoding>;

    template <typename TInput>
    export using AsciiToAsciiLazyConverter = _CharEncodingLazyConverterHelper<
        _AsciiToAsciiConversionImpl, AsciiEncoding, AsciiEncoding, TInput>;
}