export module atom.core:range.mut_bidi_range_extensions;
import :range.bidi_range_extensions;
import :range.mut_fwd_range_extensions;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _tconst_range_extensions_impl = void>
    class _mut_bidi_range_extensions_impl
        : public _mut_fwd_range_extensions_impl<range_type, _tconst_range_extensions_impl>
    {
        using base_type = _mut_fwd_range_extensions_impl<range_type, _tconst_range_extensions_impl>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class _mut_bidi_range_extensions_impl<range_type, void>
        : public _mut_bidi_range_extensions_impl<range_type,
              _bidi_range_extensions_impl<range_type>>
    {
        using base_type =
            _mut_bidi_range_extensions_impl<range_type, _bidi_range_extensions_impl<range_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _tconst_range_extensions = void>
    class mut_bidi_range_extensions
        : public mut_fwd_range_extensions<range_type, _tconst_range_extensions>
    {
        using base_type = mut_fwd_range_extensions<range_type, _tconst_range_extensions>;

    protected:
        using _impl_type = typename base_type::_impl_type;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;
        using mut_iter_type = typename base_type::mut_iter_type;
        using mut_iter_end_type = typename base_type::mut_iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename range_type>
    class mut_bidi_range_extensions<range_type, void>
        : public mut_bidi_range_extensions<range_type,
              bidi_range_extensions<range_type, _mut_bidi_range_extensions_impl<range_type>>>
    {
        using base_type = mut_bidi_range_extensions<range_type,
            bidi_range_extensions<range_type, _mut_bidi_range_extensions_impl<range_type>>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
