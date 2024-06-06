export module atom.core:ranges.array_iterator;

import std;
import :core;
import :types;
import :ranges.iterator_requirements;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// array_iterator iteratorates over raw arr.
    ///
    /// @todo update docs.
    /// --------------------------------------------------------------------------------------------
    export template <typename in_value_type>
    class array_iterator
    {
        static_assert(typeinfo<in_value_type>::is_pure, "array_iterator supports only pure types.");
        static_assert(not typeinfo<in_value_type>::is_void, "array_iterator does not support void.");

        using this_type = array_iterator<in_value_type>;

    public:
        using value_type = in_value_type;
        using size_t = std::size_t;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::contiguous_iterator_tag;
        using pointer = const value_type*;
        using reference = const value_type&;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_iterator()
            : _it{ nullptr }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_iterator(const value_type* it)
            : _it{ it }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// access value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() const -> const value_type&
        {
            return *_it;
        }

        /// ----------------------------------------------------------------------------------------
        /// access value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() const -> const value_type*
        {
            return _it;
        }

        /// ----------------------------------------------------------------------------------------
        /// access value by index.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator[](isize i) const -> const value_type&
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
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// move fwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        template <typename self_type>
        constexpr auto operator++(this self_type& self, int) -> self_type
        {
            self_type tmp{ const_cast<self_type::value_type*>(self._it) };
            ++self._it;
            return tmp;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iterator is same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator==(const this_type& that) const -> bool
        {
            return _it == that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iterator is not same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator!=(const this_type& that) const -> bool
        {
            return _it != that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iterator is same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator<(const this_type& that) const -> bool
        {
            return _it < that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iterator is same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator<=(const this_type& that) const -> bool
        {
            return _it <= that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iterator is same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator>(const this_type& that) const -> bool
        {
            return _it > that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iterator is same as `that`. used to compare wth end.
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
        constexpr auto operator--(this self_type& self, int) -> self_type
        {
            self_type tmp{ const_cast<self_type::value_type*>(self._it) };
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
        const value_type* _it;
    };

    export template <typename value_type>
    constexpr auto operator+(
        isize steps, const array_iterator<value_type>& it) -> array_iterator<value_type>
    {
        return it + steps;
    }

    /// --------------------------------------------------------------------------------------------
    /// mut_array_iterator iteratorates over mut raw arr.
    ///
    /// @todo update docs.
    /// --------------------------------------------------------------------------------------------
    export template <typename in_value_type>
    class mut_array_iterator: public array_iterator<in_value_type>
    {
        static_assert(
            typeinfo<in_value_type>::is_pure, "mut_array_iterator doesn't support non pure types.");
        static_assert(not typeinfo<in_value_type>::is_void, "mut_array_iterator does not support void.");

        using this_type = mut_array_iterator<in_value_type>;
        using base_type = array_iterator<in_value_type>;

    public:
        using value_type = in_value_type;
        using size_t = std::size_t;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::contiguous_iterator_tag;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr mut_array_iterator()
            : base_type{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr mut_array_iterator(value_type* it)
            : base_type{ it }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// access value by mut ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() const -> value_type&
        {
            return const_cast<value_type&>(*this->_it);
        }

        /// ----------------------------------------------------------------------------------------
        /// access value by mut ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() const -> value_type*
        {
            return const_cast<value_type*>(this->_it);
        }

        /// ----------------------------------------------------------------------------------------
        /// access value by index.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator[](isize i) const -> value_type&
        {
            return const_cast<value_type&>(this->_it[i]);
        }
    };

    export template <typename value_type>
    constexpr auto operator+(
        isize steps, const mut_array_iterator<value_type>& it) -> mut_array_iterator<value_type>
    {
        return it + steps;
    }
}
