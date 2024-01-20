export module atom.core.text:encodings.ascii_to_ascii;

namespace atom
{
    template <>
    class _ascii_to_ascii_conversion_impl
    {
    public:
        template <typename tin, typename output_type>
        constexpr void convert_char(const tin& in, output_type& out)
        {}
    };

    using ascii_to_ascii_converter =
        _char_encoding_converter_helper<_ascii_to_ascii_conversion_impl, ascii_encoding,
            ascii_encoding>;

    template <typename input_type>
    using ascii_to_ascii_lazy_converter =
        _char_encoding_lazy_converter_helper<_ascii_to_ascii_conversion_impl, ascii_encoding,
            ascii_encoding, input_type>;
}
