#pragma once
#include "atom/core.h"
// #include "atom/tti.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// array_iter iterates over raw arr.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class array_iter
    {
        static_assert(tti::is_pure<type>, "array_iter supports only pure types.");
        static_assert(not tti::is_void<type>, "array_iter does not support void.");

    public:
        using elem_type = type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_iter()
            : _it{ nullptr }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_iter(mem_ptr<type> it)
            : _it{ it }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// access value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto value() const -> const type&
        {
            return _it.get();
        }

        /// ----------------------------------------------------------------------------------------
        /// access value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto data() const -> mem_ptr<type>
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
        constexpr auto eq(const array_iter& that) const -> bool
        {
            return this->_it == that._it;
        }

        /// ----------------------------------------------------------------------------------------
        /// check if this iter is not same as `that`. used to compare wth end.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ne(const array_iter& that) const -> bool
        {
            return not eq(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// compares `this` data with `that` data.
        /// ----------------------------------------------------------------------------------------
        constexpr auto compare(const array_iter& that) const -> isize
        {
            return this->_it - that._it;
        }

    protected:
        mem_ptr<type> _it;
    };

    /// --------------------------------------------------------------------------------------------
    /// mut_array_iter iterates over mut raw arr.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_array_iter: public array_iter<type>
    {
        static_assert(tti::is_pure<type>, "mut_array_iter supports only pure types.");
        static_assert(not tti::is_void<type>, "mut_array_iter does not support void.");

    private:
        using base_type = array_iter<type>;

    public:
        using elem_type = type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr mut_array_iter()
            : base_type{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr mut_array_iter(mut_mem_ptr<type> it)
            : base_type{ it }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// access value by mut ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_value() -> type&
        {
            return const_cast<type&>(base_type::value());
        }

        /// ----------------------------------------------------------------------------------------
        /// access value by mut ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_data() -> mut_mem_ptr<type>
        {
            return const_cast<mut_mem_ptr<type>>(base_type::data());
        }

        /// ----------------------------------------------------------------------------------------
        /// move fwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        constexpr auto next() -> mut_array_iter&
        {
            base_type::next();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// move fwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto next(usize steps) -> mut_array_iter&
        {
            base_type::next(steps);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// move bwd by 1 step.
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev() -> mut_array_iter&
        {
            base_type::prev();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// move bwd by `steps`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev(usize steps) -> mut_array_iter&
        {
            base_type::prev(steps);
            return *this;
        }
    };
}
