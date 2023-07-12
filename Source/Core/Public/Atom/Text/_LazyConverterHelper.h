export module Atom.Core.Text:Encodings._LazyConverterHelper;

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// Requirements for {_CharEncodingLazyConverterHelper} API.
    /// --------------------------------------------------------------------------------------------
    template <typename TConverter, typename TInEncoding, typename TOutEncoding>
    concept RCharEncodingLazyConverter = requires
    {
        requires RRangeOf<TConverter, BasicChar<TOutEncoding>>;
        requires RMultiPassRangeOf<TConverter, BasicChar<TOutEncoding>>;
    }

    /// --------------------------------------------------------------------------------------------
    /// Ensures {_CharEncodingLazyConverterHelper<TInEncoding, TOutEncoding>} satisfies
    /// {RCharEncodingLazyConverter<TInEncoding, TOutEncoding>}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInEncoding, typename TOutEncoding>
    concept RCharEncodingLazyConvertible = RCharEncodingLazyConverter<
        _CharEncodingLazyConverterHelper<TInEncoding, TOutEncoding, Internal::FwdIterMock<BasicChar<TInEncoding>>>,
        TInEncoding, TOutEncoding>;
}

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// Converts data from {TInEncoding} character encoding to {TOutEncoding} on demand.
    /// This doesn't process the whole string, only the requested part.
    /// --------------------------------------------------------------------------------------------
    template <typename TImpl, typename TInEncoding, typename TOutEncoding, typename TInput>
    struct _CharEncodingLazyConverterHelper
    {
        using TIter = _CharEncodingLazyConverterHelperIter<
            TImpl, TInEncoding, TOutEncoding, TInput>;

        using TIterEnd = _CharEncodingLazyConverterHelperIterEnd;

        constexpr TIter Iter() noex
        {
            return TIter(_impl, _input);
        }

        constexpr TIter IterEnd() noex
        {
            return TIterEnd();
        }

        constexpr TIter begin() noex
        {
            return Iter();
        }

        constexpr TIterEnd end() noex
        {
            return IterEnd();
        }

    protected:
        TImpl _impl;
        TInput& _input;
    };

    struct _CharEncodingLazyConverterHelperIterEnd { };

    /// --------------------------------------------------------------------------------------------
    /// Converts data from {TInEncoding} character encoding to {TOutEncoding} on demand.
    /// This doesn't process the whole string, only the requested part.
    /// --------------------------------------------------------------------------------------------
    template <typename TImpl, typename TInEncoding, typename TOutEncoding, typename TInput>
    struct _CharEncodingLazyConverterHelperIter
    {
        using TThis = _CharEncodingLazyConverterHelperIter;
        using TIterEnd = _CharEncodingLazyConverterHelperIterEnd;
        using TInChar = typename TInEncoding::TChar;
        using TOutChar = typename TOutEncoding::TChar;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr _CharEncodingLazyConverterHelperIter(TImpl&& impl, TInput&& in) noex:
            _impl{ FORWARD(impl) }, _input{ FORWARD(input) }, _out{ 0 }, _outIndex{ -1 }
        {
            _ProcessNextChar();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the current char.
        /// ----------------------------------------------------------------------------------------
        constexpr TOutChar& operator* () noex
        {
            return _out[_outIndex];
        }

        /// ----------------------------------------------------------------------------------------
        /// Advances the iter.
        /// ----------------------------------------------------------------------------------------
        constexpr TThis& operator++ ()
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
        constexpr TThis& operator++ (int)
        {
            return ++(*this);
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if the iter has reached its end.
        /// ----------------------------------------------------------------------------------------
        constexpr bool operator== (TIterEnd end) const noex
        {
            return _outIndex > 0 || _input.HasNext();
        }

        /// ----------------------------------------------------------------------------------------
        /// Will be removed in CPP2;
        /// ----------------------------------------------------------------------------------------
        constexpr bool operator!= (TIterEnd end) const noex
        {
            return !(*this == end);
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        void _ProcessNextChar()
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
        using TChar = BasicChar<TCharEncoding>;

    public:
        constexpr _CharEncodingLazyConverterHelper(TInput&& input) noex:
            _input{ input } { }

    public:
        constexpr TChar Get() noex
        {
            return _input.Get();
        }

        constexpr bool Next()
        {
            return _input.Next();
        }

        constexpr bool HasNext() const noex
        {
            return _input.HasNext();
        }

    protected:
        TInput _input;
    };
};