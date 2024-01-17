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
        using tin_char = typename tin_encoding::tchar;
        using tout_char = typename tout_encoding::tchar;
        using tout_string = internal::string<tout_encoding>;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tinput, typename tout>
            requires(rrange_of<tinput, tin_char>) and (routput<tout, tout_char>)
        constexpr auto convert_to(const tinput& in, tout out)
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
    template <typename impl_type, typename tencoding>
    class _char_encoding_converter_helper<impl_type, tencoding, tencoding>
    {
    public:
        using tchar = basic_char<tchar_encoding>;
        using string_type = basic_string<tchar_encoding>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// writes input to output as is.
        /// ----------------------------------------------------------------------------------------
        template <typename tinput, typename tout>
            requires(rrange_of<tinput, const tchar>) and (routput<tout, tchar>)
        constexpr auto convert(tinput&& in, tout& out)
        {
            out += forward<tinput>(in);
        }
    };
}
