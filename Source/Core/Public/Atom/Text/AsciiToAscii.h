export module Atom.Core.Text:Encodings.AsciiToAscii;

namespace Atom
{
    template < >
    class _AsciiToAsciiConversionImpl
    {
        pub template <tname TIn, tname TOut>
        cexpr void ConvertChar(const TIn& in, TOut& out)
        {
        }
    };

    using AsciiToAsciiConverter = _CharEncodingConverterHelper<
        _AsciiToAsciiConversionImpl, AsciiEncoding, AsciiEncoding>;

    template <tname TInput>
    using AsciiToAsciiLazyConverter = _CharEncodingLazyConverterHelper<
        _AsciiToAsciiConversionImpl, AsciiEncoding, AsciiEncoding, TInput>;
}