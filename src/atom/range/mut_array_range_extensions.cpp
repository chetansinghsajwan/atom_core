export module atom.core:range.mut_array_range_extensions;
import :range.array_range_extensions;
import :range.mut_jump_range_extensions;
import :contracts_decl;
import :int_wrapper;
import :mem_ptr;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _tconst_range_extensions_impl = void>
    class _mut_array_range_extensions_impl
        : public _mut_jump_range_extensions_impl<range_type, _tconst_range_extensions_impl>
    {
        using base_type = _mut_jump_range_extensions_impl<range_type, _tconst_range_extensions_impl>;

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

    public:
        constexpr auto mut_data() -> mut_mem_ptr<elem_type>
        {
            return _range().mut_data();
        }

        constexpr auto count() const -> usize
        {
            return _range().count();
        }

        constexpr auto mut_at(usize i) -> elem_type&
        {
            return (mut_data() + i).get_mut();
        }

        constexpr auto mut_front() -> elem_type&
        {
            return mut_at(0);
        }

        constexpr auto mut_back() -> elem_type&
        {
            contracts::debug_expects(count() > 0);

            return mut_at(count() - 1);
        }

        constexpr auto mut_iter(usize i) -> mut_iter_type
        {
            return base_type::mut_iter().next(i);
        }

        using base_type::mut_iter;

    protected:
        using base_type::_range;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class _mut_array_range_extensions_impl<range_type, void>
        : public _mut_array_range_extensions_impl<range_type, _array_range_extensions_impl<range_type>>
    {
        using base_type = _mut_array_range_extensions_impl<range_type, _array_range_extensions_impl<range_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _tconst_range_extensions = void>
    class mut_array_range_extensions: public mut_jump_range_extensions<range_type, _tconst_range_extensions>
    {
        using base_type = mut_jump_range_extensions<range_type, _tconst_range_extensions>;

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

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// access
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// get underlying ptr to arr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_data() -> mut_mem_ptr<elem_type>
        {
            return _impl().mut_data();
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
        constexpr auto mut_at(usize i) -> elem_type&
        {
            contracts::expects(is_index_in_range(i), "index is out of range.");

            return _impl().mut_at(i);
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
        constexpr auto operator[](usize i) -> elem_type&
        {
            contracts::debug_expects(is_index_in_range(i), "index is out of range.");

            return _impl().mut_at(i);
        }

        using base_type::operator[];

        /// ----------------------------------------------------------------------------------------
        /// access first element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_front() -> elem_type&
        {
            contracts::debug_expects(not is_empty(), "range is empty.");

            return _impl().mut_front();
        }

        /// ----------------------------------------------------------------------------------------
        /// access last element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_back() -> elem_type&
        {
            contracts::debug_expects(not is_empty(), "range is empty.");

            return _impl().mut_back();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// get iter to element at index `i`.
        ///
        /// # parameters
        ///
        /// - `i`: index of the element to get iter at.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_iter(usize i) const -> mut_iter_type
        {
            contracts::expects(is_index_in_range(i), "index is out of range.");

            return _impl().mut_iter(i);
        }

        using base_type::mut_iter;

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// view
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

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

        using base_type::is_empty;
        using base_type::is_index_in_range;

    protected:
        using base_type::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename range_type>
    class mut_array_range_extensions<range_type, void>
        : public mut_array_range_extensions<range_type,
              array_range_extensions<range_type, _mut_array_range_extensions_impl<range_type>>>
    {
        using base_type = mut_array_range_extensions<range_type,
            array_range_extensions<range_type, _mut_array_range_extensions_impl<range_type>>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
