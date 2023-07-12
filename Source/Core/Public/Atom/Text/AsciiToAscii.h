export module Atom.Core.Text:Encodings.AsciiToAscii;

namespace Atom
{
    template < >
    struct _AsciiToAsciiConversionImpl
    {
        template <tname TIn, tname TOut>
        cexpr void ConvertChar(const TIn& in, TOut& out)
        {
        }
    };

    export using AsciiToAsciiConverter = _CharEncodingConverterHelper<
        _AsciiToAsciiConversionImpl, AsciiEncoding, AsciiEncoding>;

    template <tname TInput>
    export using AsciiToAsciiLazyConverter = _CharEncodingLazyConverterHelper<
        _AsciiToAsciiConversionImpl, AsciiEncoding, AsciiEncoding, TInput>;
}