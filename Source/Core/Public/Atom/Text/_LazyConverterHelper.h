export module Atom.Core.Text:Encodings._LazyConverterHelper;

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// Requirements for {_CharEncodingLazyConverterHelper} API.
    /// --------------------------------------------------------------------------------------------
    template <tname TConverter, tname TInEncoding, tname TOutEncoding>
    concept RCharEncodingLazyConverter = requires
    {
        requires RRangeOf<TConverter, BasicChar<TOutEncoding>>;
        requires RMultiPassRangeOf<TConverter, BasicChar<TOutEncoding>>;
    }

    /// --------------------------------------------------------------------------------------------
    /// Ensures {_CharEncodingLazyConverterHelper<TInEncoding, TOutEncoding>} satisfies
    /// {RCharEncodingLazyConverter<TInEncoding, TOutEncoding>}.
    /// --------------------------------------------------------------------------------------------
    template <tname TInEncoding, tname TOutEncoding>
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
    template <tname TImpl, tname TInEncoding, tname TOutEncoding, tname TInput>
    struct _CharEncodingLazyConverterHelper
    {
        using TIter = _CharEncodingLazyConverterHelperIter<
            TImpl, TInEncoding, TOutEncoding, TInput>;

        using TIterEnd = _CharEncodingLazyConverterHelperIterEnd;

        cexpr TIter Iter() noex
        {
            return TIter(_impl, _input);
        }

        cexpr TIter IterEnd() noex
        {
            return TIterEnd();
        }

        cexpr TIter begin() noex
        {
            return Iter();
        }

        cexpr TIterEnd end() noex
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
    template <tname TImpl, tname TInEncoding, tname TOutEncoding, tname TInput>
    struct _CharEncodingLazyConverterHelperIter
    {
        using TThis = _CharEncodingLazyConverterHelperIter;
        using TIterEnd = _CharEncodingLazyConverterHelperIterEnd;
        using TInChar = tname TInEncoding::TChar;
        using TOutChar = tname TOutEncoding::TChar;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr _CharEncodingLazyConverterHelperIter(TImpl&& impl, TInput&& in) noex:
            _impl{ fwd(impl) }, _input{ fwd(input) }, _out{ 0 }, _outIndex{ -1 }
        {
            _ProcessNextChar();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the current char.
        /// ----------------------------------------------------------------------------------------
        cexpr TOutChar& operator* () noex
        {
            return _out[_outIndex];
        }

        /// ----------------------------------------------------------------------------------------
        /// Advances the iter.
        /// ----------------------------------------------------------------------------------------
        cexpr TThis& operator++ ()
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
        cexpr TThis& operator++ (int)
        {
            return ++(*this);
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if the iter has reached its end.
        /// ----------------------------------------------------------------------------------------
        cexpr bool operator== (TIterEnd end) const noex
        {
            return _outIndex > 0 || _input.HasNext();
        }

        /// ----------------------------------------------------------------------------------------
        /// Will be removed in CPP2;
        /// ----------------------------------------------------------------------------------------
        cexpr bool operator!= (TIterEnd end) const noex
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
    template <tname TImpl, tname TCharEncoding, tname TInput>
    class _CharEncodingLazyConverterHelper<TImpl, TCharEncoding, TCharEncoding, TInput>
    {
        using TChar = BasicChar<TCharEncoding>;

    public:
        cexpr _CharEncodingLazyConverterHelper(TInput&& input) noex:
            _input{ input } { }

    public:
        cexpr TChar Get() noex
        {
            return _input.Get();
        }

        cexpr bool Next()
        {
            return _input.Next();
        }

        cexpr bool HasNext() const noex
        {
            return _input.HasNext();
        }

    protected:
        TInput _input;
    };
};