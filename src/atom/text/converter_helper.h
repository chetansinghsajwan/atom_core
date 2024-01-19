export module atom.core.text:encodings._converter_helper;

namespace atom::text
{
    /// --------------------------------------------------------------------------------------------
    /// requirements for {char_encoding_converter} api.
    /// --------------------------------------------------------------------------------------------
    export template <typename tconverter, typename tin_encoding, typename tout_encoding>
    concept rchar_encoding_converter =
        requires(tconverter converter, internal::range_mock<basic_char<tin_encoding>> in,
            internal::output_mock<basic_char<tout_encoding>> out) {
            requires rdefault_constructible<tconverter>;

            {
                converter.convert(in)
            } -> rsame_as<basic_string<tout_encoding>>;

            {
                converter.convert(in, out)
            } -> rsame_as<void>;
        };

    /// --------------------------------------------------------------------------------------------
    /// ensures {char_encoding_converter<tin_encoding, tout_encoding>} satisfies
    /// {rchar_encoding_converter<tin_encoding, tout_encoding>}.
    /// --------------------------------------------------------------------------------------------
    export template <typename tin_encoding, typename tout_encoding>
    concept rchar_encoding_convertible =
        rchar_encoding_converter<char_encoding_converter<tin_encoding, tout_encoding>, tin_encoding,
            tout_encoding>;
}

namespace atom::text
{
    /// --------------------------------------------------------------------------------------------
    /// converts data from {tin_encoding} character encoding to {tout_encoding}.
    ///
    /// @tparam impl_type conversion impl.
    /// @tparam tin_encoding character encoding to convert data from.
    /// @tparam tout_encoding character encoding to convert data to.
    /// --------------------------------------------------------------------------------------------
    template <typename impl_type, typename tin_encoding, typename tout_encoding>
    class _char_encoding_converter_helper
    {
    public:
        using tin_char = typename tin_encoding::char_type;
        using tout_char = typename tout_encoding::char_type;
        using tout_string = internal::string<tout_encoding>;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename range_type, typename output_type>
            requires(rrange_of<range_type, tin_char>) and (routput<output_type, tout_char>)
        constexpr auto convert_to(const range_type& in, output_type out)
        {
            auto end = in.end();
            for (auto it = in.begin(); it != end; it++)
            {
                impl_type::convert_char(it, out);
            }
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// specialization for same {char_encoding}.
    /// --------------------------------------------------------------------------------------------
    template <typename impl_type, typename encoding_type>
    class _char_encoding_converter_helper<impl_type, encoding_type, encoding_type>
    {
    public:
        using char_type = basic_char<tchar_encoding>;
        using string_type = basic_string<tchar_encoding>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// writes input to output as is.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type, typename output_type>
        constexpr auto convert(range_type&& in, output_type& out)
            requires(rrange_of<range_type, const char_type>) and (routput<output_type, char_type>)
        {
            out += forward<range_type>(in);
        }
    };
}
