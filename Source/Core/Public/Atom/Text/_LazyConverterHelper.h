export module Atom.Core.Text:Encodings._LazyConverterHelper;

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// Requirements for {_CharEncodingLazyConverterHelper} API.
    /// --------------------------------------------------------------------------------------------
    template <typename TConverter, typename TInEncoding, typename TOutEncoding>
    concept RCharEncodingLazyConverter =
        requires {
            requires RRangeOf<TConverter, BasicChar<TOutEncoding>>;
            requires RMultiPassRangeOf<TConverter, BasicChar<TOutEncoding>>;
        }

    /// --------------------------------------------------------------------------------------------
    /// Ensures {_CharEncodingLazyConverterHelper<TInEncoding, TOutEncoding>} satisfies
    /// {RCharEncodingLazyConverter<TInEncoding, TOutEncoding>}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInEncoding, typename TOutEncoding>
    concept RCharEncodingLazyConvertible =
        RCharEncodingLazyConverter<_CharEncodingLazyConverterHelper<TInEncoding, TOutEncoding,
                                       Internal::FwdIterMock<BasicChar<TInEncoding>>>,
            TInEncoding, TOutEncoding>;
}

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// Converts data from {TInEncoding} character encoding to {TOutEncoding} on demand.
    /// This doesn't process the whole string, only the requested part.
    /// --------------------------------------------------------------------------------------------
    template <typename TImpl, typename TInEncoding, typename TOutEncoding, typename TInput>
    class _CharEncodingLazyConverterHelper
    {
    public:
        using TIter =
            _CharEncodingLazyConverterHelperIter<TImpl, TInEncoding, TOutEncoding, TInput>;

        using TIterEnd = _CharEncodingLazyConverterHelperIterEnd;

    public:
        constexpr auto iter() -> TIter
        {
            return TIter(_impl, _input);
        }

        constexpr auto iterEnd() -> TIter
        {
            return TIterEnd();
        }

        constexpr auto begin() -> TIter
        {
            return iter();
        }

        constexpr auto end() -> TIterEnd
        {
            return iterEnd();
        }

    protected:
        TImpl _impl;
        TInput& _input;
    };

    class _CharEncodingLazyConverterHelperIterEnd
    {};

    /// --------------------------------------------------------------------------------------------
    /// Converts data from {TInEncoding} character encoding to {TOutEncoding} on demand.
    /// This doesn't process the whole string, only the requested part.
    /// --------------------------------------------------------------------------------------------
    template <typename TImpl, typename TInEncoding, typename TOutEncoding, typename TInput>
    class _CharEncodingLazyConverterHelperIter
    {
    public:
        using TThis = _CharEncodingLazyConverterHelperIter;
        using TIterEnd = _CharEncodingLazyConverterHelperIterEnd;
        using TInChar = typename TInEncoding::TChar;
        using TOutChar = typename TOutEncoding::TChar;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr _CharEncodingLazyConverterHelperIter(TImpl&& impl, TInput&& in)
            : _impl{ fwd(impl) }
            , _input{ fwd(input) }
            , _out{ 0 }
            , _outIndex{ -1 }
        {
            _ProcessNextChar();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the current char.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() -> TOutChar&
        {
            return _out[_outIndex];
        }

        /// ----------------------------------------------------------------------------------------
        /// Advances the iter.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator++() -> TSelf&
        {
            if (_outIndex == -1)
            {
                _ProcessNextChar();
            }

            _outIndex--;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Will be removed in CPP2;
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator++(i32) -> TSelf&
        {
            return ++*this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if the iter has reached its end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator==(TIterEnd end) const -> bool
        {
            return _outIndex > 0 || _input.HasNext();
        }

        /// ----------------------------------------------------------------------------------------
        /// Will be removed in CPP2;
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator!=(TIterEnd end) const -> bool
        {
            return !(*this == end);
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto _ProcessNextChar()
        {
            if (_inIter == _inIterEnd)
            {
                throw 0;
            }

            _outRune = TImpl::ConvertChar(*_inIter);
        }

    protected:
        TImpl _impl;
        TInIter _inIter;
        TInIterEnd _inIterEnd;

        TInRune _inRune;
        TOutRune _outRune;
    };

    /// --------------------------------------------------------------------------------------------
    /// {_CharEncodingLazyConverterHelper} specialization for same character encodings.
    /// --------------------------------------------------------------------------------------------
    template <typename TImpl, typename TCharEncoding, typename TInput>
    class _CharEncodingLazyConverterHelper<TImpl, TCharEncoding, TCharEncoding, TInput>
    {
    private:
        using TChar = BasicChar<TCharEncoding>;

    public:
        constexpr _CharEncodingLazyConverterHelper(TInput&& input)
            : _input{ input }
        {}

    public:
        constexpr auto Get() -> TChar
        {
            return _input.Get();
        }

        constexpr auto Next() -> bool
        {
            return _input.Next();
        }

        constexpr auto HasNext() const -> bool
        {
            return _input.HasNext();
        }

    protected:
        TInput _input;
    };
};
