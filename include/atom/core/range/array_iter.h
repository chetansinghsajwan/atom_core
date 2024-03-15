#pragma once
#include "atom/core/core.h"
#include "atom/core/typeinfo.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// array_iter iterates over raw arr.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class array_iter
    {
        ATOM_STATIC_ASSERTS(typeinfo::is_pure<in_value_t>, "array_iter supports only pure types.");
        ATOM_STATIC_ASSERTS(not typeinfo::is_void<in_value_t>, "array_iter does not support void.");

    public:
        using value_t = in_value_t;

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
        constexpr auto value() const -> const value_t&
        {
            return *_it;
        }

        /// ----------------------------------------------------------------------------------------
        /// access value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data() const -> const value_t*
        {
            return this->_it;
        }

        /// ----------------------------------------------------------------------------------------
        /// move fwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        constexpr auto next() -> array_iter&
        {
            _it++;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// move fwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto next(usize steps) -> array_iter&
        {
            _it += steps;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// move bwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev() -> array_iter&
        {
            _it--;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// move bwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev(usize steps) -> array_iter&
        {
            _it -= steps;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iter is same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_eq(const array_iter& that) const -> bool
        {
            return this->_it == that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// compares `this` data with `that` data.
        /// ----------------------------------------------------------------------------------------
        constexpr auto compare(const array_iter& that) const -> isize
        {
            return this->_it - that._it;
        }

    protected:
        const value_t* _it;
    };

    /// --------------------------------------------------------------------------------------------
    /// mut_array_iter iterates over mut raw arr.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class mut_array_iter: public array_iter<in_value_t>
    {
        ATOM_STATIC_ASSERTS(
            typeinfo::is_pure<in_value_t>, "mut_array_iter supports only pure types.");
        ATOM_STATIC_ASSERTS(
            not typeinfo::is_void<in_value_t>, "mut_array_iter does not support void.");

        using base_t = array_iter<in_value_t>;

    public:
        using value_t = in_value_t;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr mut_array_iter()
            : base_t()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr mut_array_iter(value_t* it)
            : base_t(it)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// access value by mut ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_value() -> value_t&
        {
            return const_cast<value_t&>(base_t::value());
        }

        /// ----------------------------------------------------------------------------------------
        /// access value by mut ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_data() -> value_t*
        {
            return const_cast<value_t*>(base_t::get_data());
        }

        /// ----------------------------------------------------------------------------------------
        /// move fwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        constexpr auto next() -> mut_array_iter&
        {
            base_t::next();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// move fwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto next(usize steps) -> mut_array_iter&
        {
            base_t::next(steps);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// move bwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev() -> mut_array_iter&
        {
            base_t::prev();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// move bwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev(usize steps) -> mut_array_iter&
        {
            base_t::prev(steps);
            return *this;
        }
    };
}
