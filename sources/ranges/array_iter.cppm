export module atom.core:ranges.array_iter;

import std;
import :core;
import :types;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// array_iter iterates over raw arr.
    ///
    /// @todo update docs.
    /// --------------------------------------------------------------------------------------------
    export template <typename in_value_t>
    class array_iter
    {
        static_assert(typeinfo<in_value_t>::is_pure, "array_iter supports only pure types.");
        static_assert(not typeinfo<in_value_t>::is_void, "array_iter does not support void.");

        using this_type = array_iter<in_value_t>;

    public:
        using value_t = in_value_t;

        using value_type = value_t;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::contiguous_iterator_tag;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_iter()
            : _it(nullptr)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_iter(const value_t* it)
            : _it(it)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// access value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() const -> const value_t&
        {
            return *_it;
        }

        /// ----------------------------------------------------------------------------------------
        /// access value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() const -> const value_t*
        {
            return _it;
        }

        /// ----------------------------------------------------------------------------------------
        /// access value by index.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator[](isize i) const -> const value_t&
        {
            return _it[i];
        }

        /// ----------------------------------------------------------------------------------------
        /// move fwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        template <typename self_type>
        constexpr auto operator++(this self_type&& self) -> self_type&
        {
            ++self._it;
            return &self;
        }

        /// ----------------------------------------------------------------------------------------
        /// move fwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        template <typename self_type>
        constexpr auto operator++(this const self_type& self, int) -> self_type
        {
            self_type tmp{ self._it };
            ++self._it;
            return tmp;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iter is same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator==(const this_type& that) const -> bool
        {
            return _it == that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iter is not same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator!=(const this_type& that) const -> bool
        {
            return _it != that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iter is same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator<(const this_type& that) const -> bool
        {
            return _it < that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iter is same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator<=(const this_type& that) const -> bool
        {
            return _it <= that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iter is same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator>(const this_type& that) const -> bool
        {
            return _it > that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iter is same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator>=(const this_type& that) const -> bool
        {
            return _it >= that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// move fwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        template <typename self_type>
        constexpr auto operator+(this const self_type& self, isize steps) -> self_type
        {
            return self_type{ self._it + steps };
        }

        /// ----------------------------------------------------------------------------------------
        /// move fwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        template <typename self_type>
        constexpr auto operator+=(this self_type&& self, isize steps) -> self_type&
        {
            self._it += steps;
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// move bwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        template <typename self_type>
        constexpr auto operator--(this self_type&& self) -> self_type&
        {
            --self._it;
            return &self;
        }

        /// ----------------------------------------------------------------------------------------
        /// move bwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        template <typename self_type>
        constexpr auto operator--(this const self_type& self, int) -> self_type
        {
            self_type tmp{ self._it };
            --self._it;
            return tmp;
        }

        /// ----------------------------------------------------------------------------------------
        /// move bwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        template <typename self_type>
        constexpr auto operator-(this const self_type& self, isize steps) -> self_type
        {
            return self_type{ self._it - steps };
        }

        /// ----------------------------------------------------------------------------------------
        /// move bwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        template <typename self_type>
        constexpr auto operator-=(this const self_type& self, isize steps) -> self_type&
        {
            self._it -= steps;
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the difference between `this` and `that`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-(const this_type& that) const -> isize
        {
            return _it - that._it;
        }

    protected:
        const value_t* _it;
    };

    template <typename value_type>
    constexpr auto operator+(isize steps, const array_iter<value_type>& it) -> array_iter<value_type>
    {
        return it + steps;
    }

    /// --------------------------------------------------------------------------------------------
    /// mut_array_iter iterates over mut raw arr.
    ///
    /// @todo update docs.
    /// --------------------------------------------------------------------------------------------
    export template <typename in_value_t>
    class mut_array_iter: public array_iter<in_value_t>
    {
        static_assert(typeinfo<in_value_t>::is_pure, "mut_array_iter supports only pure types.");
        static_assert(not typeinfo<in_value_t>::is_void, "mut_array_iter does not support void.");

        using this_type = mut_array_iter<in_value_t>;
        using base_t = array_iter<in_value_t>;

    public:
        using value_t = in_value_t;
        using value_type = value_t;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr mut_array_iter()
            : base_t{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr mut_array_iter(value_t* it)
            : base_t{ it }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// access value by mut ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() -> value_t&
        {
            return const_cast<value_t&>(*this->_it);
        }

        using base_t::operator*;

        /// ----------------------------------------------------------------------------------------
        /// access value by mut ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() -> value_t*
        {
            return const_cast<value_t*>(this->_it);
        }

        using base_t::operator->;

        /// ----------------------------------------------------------------------------------------
        /// access value by index.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator[](isize i) const -> value_t&
        {
            return this->_it[i];
        }

        using base_t::operator[];
    };

    template <typename value_type>
    constexpr auto operator+(isize steps, const mut_array_iter<value_type>& it) -> mut_array_iter<value_type>
    {
        return it + steps;
    }
}
