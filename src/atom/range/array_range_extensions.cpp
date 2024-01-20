export module atom.core:range.array_range_extensions;
import :range.jump_range_extensions;
import :contracts_decl;
import :int_wrapper;
import :mem_ptr;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// implementation of [`array_range_extensions`].
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class _array_range_extensions_impl: public _jump_range_extensions_impl<range_type>
    {
        using base_type = _jump_range_extensions_impl<range_type>;

    protected:
        using _timpl = typename base_type::_timpl;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        constexpr auto data() const -> mem_ptr<elem_type>
        {
            return _range().data();
        }

        constexpr auto count() const -> usize
        {
            return _range().count();
        }

        constexpr auto at(usize i) const -> const elem_type&
        {
            contracts::debug_expects(is_index_in_range(i));

            return data()[i];
        }

        constexpr auto front() const -> const elem_type&
        {
            return at(0);
        }

        constexpr auto back() const -> const elem_type&
        {
            return back(count() - 1);
        }

        constexpr auto is_index_in_range(usize i) const -> bool
        {
            return i < count();
        }

        constexpr auto iter(usize i) const -> iter_type
        {
            return base_type::iter().next(i);
        }

        using base_type::iter;

    protected:
        using base_type::_range;
    };

    /// --------------------------------------------------------------------------------------------
    /// [array_range_extensions].
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _trange_extensions_impl_ = void>
    class array_range_extensions: public jump_range_extensions<range_type, _trange_extensions_impl_>
    {
        using base_type = jump_range_extensions<range_type, _trange_extensions_impl_>;

    protected:
        using _timpl = typename base_type::_timpl;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// access
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// get underlying ptr to arr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto data() const -> mem_ptr<elem_type>
        {
            return _impl().data();
        }

        /// ----------------------------------------------------------------------------------------
        /// access element by index.
        ///
        /// # parameters
        /// - `i`: index of element to access.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto at(usize i) const -> const elem_type&
        {
            contracts::expects(is_index_in_range(i), "index is out of range.");

            return _impl().at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// access element by index.
        ///
        /// # parameters
        /// - `i`: index of element to access.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator[](usize i) const -> const elem_type&
        {
            contracts::debug_expects(is_index_in_range(i), "index is out of range.");

            return _impl().at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// access first element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto front() const -> const elem_type&
        {
            contracts::debug_expects(not is_empty(), "range is empty.");

            return _impl().front();
        }

        /// ----------------------------------------------------------------------------------------
        /// access last element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto back() const -> const elem_type&
        {
            contracts::debug_expects(not is_empty(), "range is empty.");

            return _impl().back();
        }

        /// ----------------------------------------------------------------------------------------
        /// get count of elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count() const -> usize
        {
            return _impl().count();
        }

        /// ----------------------------------------------------------------------------------------
        /// is range empty.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_empty() const -> bool
        {
            return _impl().count() == 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// get iter to element at index `i`.
        ///
        /// # parameters
        ///
        /// - `i`: index of the element to get iter at.
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter(usize i) const -> iter_type
        {
            contracts::expects(is_index_in_range(i), "index is out of range.");

            return _impl().iter(i);
        }

        using base_type::iter;

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// view
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// # to do: review this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto range(usize from, usize to) const {}

        /// ----------------------------------------------------------------------------------------
        /// # to do: review this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto range_from(usize from) const {}

        /// ----------------------------------------------------------------------------------------
        /// # to do: review this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto range_to(usize to) const {}

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// validation
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        constexpr auto is_index_in_range(usize i) const -> bool
        {
            return _impl().is_index_in_range(i);
        }

    protected:
        using base_type::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename range_type>
    class array_range_extensions<range_type, void>
        : public array_range_extensions<range_type, _array_range_extensions_impl<range_type>>
    {
        using base_type = array_range_extensions<range_type, _array_range_extensions_impl<range_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
