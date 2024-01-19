export module atom.core.text:encodings._lazy_converter_helper;

namespace atom::text
{
    /// --------------------------------------------------------------------------------------------
    /// requirements for {_char_encoding_lazy_converter_helper} api.
    /// --------------------------------------------------------------------------------------------
    template <typename tconverter, typename tin_encoding, typename tout_encoding>
    concept rchar_encoding_lazy_converter =
        requires {
            requires rrange_of<tconverter, basic_char<tout_encoding>>;
            requires rmulti_pass_range_of<tconverter, basic_char<tout_encoding>>;
        }

    /// --------------------------------------------------------------------------------------------
    /// ensures {_char_encoding_lazy_converter_helper<tin_encoding, tout_encoding>} satisfies
    /// {rchar_encoding_lazy_converter<tin_encoding, tout_encoding>}.
    /// --------------------------------------------------------------------------------------------
    template <typename tin_encoding, typename tout_encoding>
    concept rchar_encoding_lazy_convertible =
        rchar_encoding_lazy_converter<_char_encoding_lazy_converter_helper<tin_encoding, tout_encoding,
                                       internal::fwd_iter_mock<basic_char<tin_encoding>>>,
            tin_encoding, tout_encoding>;
}

namespace atom::text
{
    /// --------------------------------------------------------------------------------------------
    /// converts data from {tin_encoding} character encoding to {tout_encoding} on demand.
    /// this_type does not process the whole string, only the requested part.
    /// --------------------------------------------------------------------------------------------
    template <typename impl_type, typename tin_encoding, typename tout_encoding, typename input_type>
    class _char_encoding_lazy_converter_helper
    {
    public:
        using iter_type =
            _char_encoding_lazy_converter_helper_iter<impl_type, tin_encoding, tout_encoding, input_type>;

        using iter_end_type = _char_encoding_lazy_converter_helper_iter_end;

    public:
        constexpr auto iter() -> iter_type
        {
            return iter_type(_impl, _input);
        }

        constexpr auto iter_end() -> iter_type
        {
            return iter_end_type();
        }

        constexpr auto begin() -> iter_type
        {
            return iter();
        }

        constexpr auto end() -> iter_end_type
        {
            return iter_end();
        }

    protected:
        impl_type _impl;
        input_type& _input;
    };

    class _char_encoding_lazy_converter_helper_iter_end
    {};

    /// --------------------------------------------------------------------------------------------
    /// converts data from {tin_encoding} character encoding to {tout_encoding} on demand.
    /// this_type does not process the whole string, only the requested part.
    /// --------------------------------------------------------------------------------------------
    template <typename impl_type, typename tin_encoding, typename tout_encoding, typename input_type>
    class _char_encoding_lazy_converter_helper_iter
    {
    public:
        using this_final_type = _char_encoding_lazy_converter_helper_iter;
        using iter_end_type = _char_encoding_lazy_converter_helper_iter_end;
        using tin_char = typename tin_encoding::char_type;
        using tout_char = typename tout_encoding::char_type;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr _char_encoding_lazy_converter_helper_iter(impl_type&& impl, input_type&& in)
            : _impl{ forward<impl_type>(impl) }
            , _input{ forward<input_type>(input) }
            , _out{ 0 }
            , _out_index{ -1 }
        {
            _process_next_char();
        }

        /// ----------------------------------------------------------------------------------------
        /// get the current char.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() -> tout_char&
        {
            return _out[_out_index];
        }

        /// ----------------------------------------------------------------------------------------
        /// advances the iter.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator++() -> self_type&
        {
            if (_out_index == -1)
            {
                _process_next_char();
            }

            _out_index--;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// will be removed in cpp2;
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator++(int) -> self_type&
        {
            return ++*this;
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if the iter has reached its end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator==(iter_end_type end) const -> bool
        {
            return _out_index > 0 || _input.has_next();
        }

        /// ----------------------------------------------------------------------------------------
        /// will be removed in cpp2;
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator!=(iter_end_type end) const -> bool
        {
            return !(*this == end);
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto _process_next_char()
        {
            if (_in_iter == _in_iter_end)
            {
                throw 0;
            }

            _out_rune = impl_type::convert_char(*_in_iter);
        }

    protected:
        impl_type _impl;
        tin_iter _in_iter;
        tin_iter_end _in_iter_end;

        tin_rune _in_rune;
        tout_rune _out_rune;
    };

    /// --------------------------------------------------------------------------------------------
    /// {_char_encoding_lazy_converter_helper} specialization for same character encodings.
    /// --------------------------------------------------------------------------------------------
    template <typename impl_type, typename tchar_encoding, typename input_type>
    class _char_encoding_lazy_converter_helper<impl_type, tchar_encoding, tchar_encoding, input_type>
    {
    private:
        using char_type = basic_char<tchar_encoding>;

    public:
        constexpr _char_encoding_lazy_converter_helper(input_type&& input)
            : _input{ input }
        {}

    public:
        constexpr auto get() -> char_type
        {
            return _input.get();
        }

        constexpr auto next() -> bool
        {
            return _input.next();
        }

        constexpr auto has_next() const -> bool
        {
            return _input.has_next();
        }

    protected:
        input_type _input;
    };
};
