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
    class _CharEncodingLazyConverterHelper
    {
        pub using TIter = _CharEncodingLazyConverterHelperIter<
            TImpl, TInEncoding, TOutEncoding, TInput>;

        pub using TIterEnd = _CharEncodingLazyConverterHelperIterEnd;

        pub cexpr fn iter() -> TIter
        {
            return TIter(_impl, _input);
        }

        pub cexpr fn iterEnd() -> TIter
        {
            return TIterEnd();
        }

        pub cexpr fn begin() -> TIter
        {
            return iter();
        }

        pub cexpr fn end() -> TIterEnd
        {
            return iterEnd();
        }

        prot TImpl _impl;
        prot TInput& _input;
    };

    class _CharEncodingLazyConverterHelperIterEnd { };

    /// --------------------------------------------------------------------------------------------
    /// Converts data from {TInEncoding} character encoding to {TOutEncoding} on demand.
    /// This doesn't process the whole string, only the requested part.
    /// --------------------------------------------------------------------------------------------
    template <typename TImpl, typename TInEncoding, typename TOutEncoding, typename TInput>
    class _CharEncodingLazyConverterHelperIter
    {
        pub using TThis = _CharEncodingLazyConverterHelperIter;
        pub using TIterEnd = _CharEncodingLazyConverterHelperIterEnd;
        pub using TInChar = typename TInEncoding::TChar;
        pub using TOutChar = typename TOutEncoding::TChar;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor _CharEncodingLazyConverterHelperIter(TImpl&& impl, TInput&& in):
            _impl{ fwd(impl) }, _input{ fwd(input) }, _out{ 0 }, _outIndex{ -1 }
        {
            _ProcessNextChar();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the current char.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op*() -> TOutChar&
        {
            return _out[_outIndex];
        }

        /// ----------------------------------------------------------------------------------------
        /// Advances the iter.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op++() -> TSelf&
        {
            if (_outIndex == -1)
            {
                _ProcessNextChar();
            }

            _outIndex--;
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// Will be removed in CPP2;
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op++(i32) -> TSelf&
        {
            return ++self;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if the iter has reached its end.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op==(TIterEnd end) const -> bool
        {
            return _outIndex > 0 || _input.HasNext();
        }

        /// ----------------------------------------------------------------------------------------
        /// Will be removed in CPP2;
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op!=(TIterEnd end) const -> bool
        {
            return !(self == end);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        prot fn _ProcessNextChar()
        {
            if (_inIter == _inIterEnd)
            {
                throw 0;
            }

            _outRune = TImpl::ConvertChar(*_inIter);
        }

        prot TImpl _impl;
        prot TInIter _inIter;
        prot TInIterEnd _inIterEnd;

        prot TInRune _inRune;
        prot TOutRune _outRune;
    };

    /// --------------------------------------------------------------------------------------------
    /// {_CharEncodingLazyConverterHelper} specialization for same character encodings.
    /// --------------------------------------------------------------------------------------------
    template <typename TImpl, typename TCharEncoding, typename TInput>
    class _CharEncodingLazyConverterHelper<TImpl, TCharEncoding, TCharEncoding, TInput>
    {
        priv using TChar = BasicChar<TCharEncoding>;

        pub cexpr ctor _CharEncodingLazyConverterHelper(TInput&& input):
            _input{ input } { }

        pub cexpr fn Get() -> TChar
        {
            return _input.Get();
        }

        pub cexpr fn Next() -> bool
        {
            return _input.Next();
        }

        pub cexpr fn HasNext() const -> bool
        {
            return _input.HasNext();
        }

        prot TInput _input;
    };
};