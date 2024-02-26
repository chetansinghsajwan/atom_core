module;
#include "atom/preprocessors.h"

export module atom.core:range.range_helper;
import :range.iter_requirements;
import :range.range_requirements;
import :range.std_iter_wrap_for_atom_iter;
import :core;
import :std;

export namespace atom
{
    class range_helper
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
        constexpr auto can_get_count() const -> bool
            requires rrange<range_type>
        {
            return rfwd_range<range_type>;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
        constexpr auto get_count(const range_type& range) const -> usize
            requires rfwd_range<range_type>
        {
            return _get_count(range.get_iter(), range.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type, class elem_type>
        constexpr auto fill(range_type&& range, elem_type&& val) const -> void
            requires rmut_range<range_type>
                     and rassignable<typename range_type::elem_type, elem_type>
        {
            _fill(range.get_iter(), range.get_iter_end(), forward<elem_type>(val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        ATOM_PRAGMA_OPTIMIZE_OFF

        template <class range_type, typename elem_type>
        constexpr auto fill_explicit(range_type&& range, elem_type&& val) const -> void
            requires rmut_range<range_type>
                     and rassignable<typename range_type::elem_type, elem_type>
        {
            _fill(range.get_iter(), range.get_iter_end(), forward<elem_type>(val));
        }

        ATOM_PRAGMA_OPTIMIZE_ON

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type_0, class range_type_1>
        constexpr auto fwd_copy_to(const range_type_0& range0, range_type_1&& range1) const -> void
            requires rrange<range_type_0> and rmut_range<range_type_1>
                     and rassignable<typename range_type_1::elem_type,
                         typename range_type_0::elem_type>
        {
            _fwd_copy(range0.get_iter(), range0.get_iter_end(), range1.get_iter(),
                range1.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type_0, class range_type_1>
        constexpr auto bwd_copy_to(const range_type_0& range0, range_type_1&& range1) const -> void
            requires rrange<range_type_0> and rmut_range<range_type_1>
                     and rassignable<typename range_type_1::elem_type,
                         typename range_type_0::elem_type>
        {
            _bwd_copy(range0.get_iter(), range0.get_iter_end(), range1.get_iter(),
                range1.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type_0, class range_type_1>
        constexpr auto fwd_move_to(range_type_0&& range0, range_type_1&& range1) const -> void
            requires rmut_range<range_type_0> and rmut_range<range_type_1>
                     and rassignable<typename range_type_1::elem_type,
                         typename range_type_0::elem_type>
        {
            _fwd_move(range0.get_iter(), range0.get_iter_end(), range1.get_iter(),
                range1.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type_0, class range_type_1>
        constexpr auto bwd_move_to(range_type_0&& range0, range_type_1&& range1) const -> void
            requires rmut_range<range_type_0> and rmut_range<range_type_1>
                     and rassignable<typename range_type_1::elem_type,
                         typename range_type_0::elem_type>
        {
            _bwd_move(range0.get_iter(), range0.get_iter_end(), range1.get_iter(),
                range1.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
        constexpr auto shift_fwd(range_type&& range, usize steps) const -> void
            requires rmut_fwd_range<range_type> and rmove_assignable<typename range_type::elem_type>
        {
            _fwd_shift(range.get_iter(), range.get_iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
        constexpr auto shift_bwd(range_type&& range, usize steps) const -> void
            requires rmut_fwd_range<range_type> and rmove_assignable<typename range_type::elem_type>
        {
            _bwd_shift(range.get_iter(), range.get_iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
        constexpr auto shift_by(range_type&& range, isize steps) const -> void
            requires rmut_fwd_range<range_type> and rswappable<typename range_type::elem_type>
        {
            if (steps > 0)
            {
                _bwd_shift(range.get_iter(), range.get_iter_end(), steps);
            }
            else
            {
                _fwd_shift(range.get_iter(), range.get_iter_end(), -steps);
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
        constexpr auto rotate_fwd(range_type&& range, usize steps) const -> void
            requires rmut_range<range_type> and rswappable<typename range_type::elem_type>
        {
            _fwd_rotate(range.get_iter(), range.get_iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
        constexpr auto rotate_bwd(range_type&& range, usize steps) const -> void
            requires rmut_range<range_type> and rswappable<typename range_type::elem_type>
        {
            _bwd_rotate(range.get_iter(), range.get_iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
        constexpr auto rotate_by(range_type&& range, isize steps) const -> void
            requires rmut_range<range_type> and rswappable<typename range_type::elem_type>
        {
            if (steps > 0)
            {
                _bwd_rotate(range.get_iter(), range.get_iter_end(), steps);
            }
            else
            {
                _fwd_rotate(range.get_iter(), range.get_iter_end(), -steps);
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
        constexpr auto destruct(range_type&& range) const -> void
            requires rmut_range<range_type> and rdestructible<typename range_type::elem_type>
        {
            _destruct(range.get_iter(), range.get_iter_end());
        }

    private:
        template <class iter_type, class iter_end_type>
        constexpr auto _get_count(iter_type iter, iter_end_type iter_end) const -> usize
        {
            if constexpr (rjump_iter_pair<iter_type, iter_end_type>)
            {
                return iter.compare(iter_end).abs().template to<usize>();
            }

            usize count = 0;
            for (; not iter.is_eq(iter_end); iter.next())
                count++;

            return count;
        }

        template <class iter_type, class iter_end_type, typename elem_type>
        constexpr auto _fill(iter_type iter, iter_end_type iter_end, elem_type&& val) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::fill(std_iter, std_iter_end, forward<elem_type>(val));
        }

        template <class iter_type_0, class iter_end_type_0, class iter_type_1,
            class iter_end_type_1>
        constexpr auto _fwd_copy(iter_type_0 iter0, iter_end_type_0 iter_end0, iter_type_1 iter1,
            iter_end_type_1 iter_end1) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter0(iter0);
            std_iter_wrap_for_atom_iter std_iter_end0(iter_end0);
            std_iter_wrap_for_atom_iter std_iter1(iter1);

            std::copy(std_iter0, std_iter_end0, std_iter1);
        }

        template <class iter_type_0, class iter_end_type_0, class iter_type_1,
            class iter_end_type_1>
        constexpr auto _bwd_copy(iter_type_0 iter0, iter_end_type_0 iter_end0, iter_type_1 iter1,
            iter_end_type_1 iter_end1) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter0(iter0);
            std_iter_wrap_for_atom_iter std_iter_end0(iter_end0);
            std_iter_wrap_for_atom_iter std_iter1(iter1);

            std::copy_backward(std_iter0, std_iter_end0, std_iter1);
        }

        template <class iter_type_0, class iter_end_type_0, class iter_type_1,
            class iter_end_type_1>
        constexpr auto _fwd_move(iter_type_0 iter0, iter_end_type_0 iter_end0, iter_type_1 iter1,
            iter_end_type_1 iter_end1) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter0(iter0);
            std_iter_wrap_for_atom_iter std_iter_end0(iter_end0);
            std_iter_wrap_for_atom_iter std_iter1(iter1);

            std::move(std_iter0, std_iter_end0, std_iter1);
        }

        template <class iter_type_0, class iter_end_type_0, class iter_type_1,
            class iter_end_type_1>
        constexpr auto _bwd_move(iter_type_0 iter0, iter_end_type_0 iter_end0, iter_type_1 iter1,
            iter_end_type_1 iter_end1) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter0(iter0);
            std_iter_wrap_for_atom_iter std_iter_end0(iter_end0);
            std_iter_wrap_for_atom_iter std_iter1(iter1);

            std::move_backward(std_iter0, std_iter_end0, std_iter1);
        }

        template <class iter_type, class iter_end_type>
        constexpr auto _fwd_shift(iter_type iter, iter_end_type iter_end, usize steps) const -> void
        {
            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
            {
                std::shift_right(iter.get_data(), iter_end.get_data(), steps);
                return;
            }

            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::shift_right(std_iter, std_iter_end, steps);
        }

        template <class iter_type, class iter_end_type>
        constexpr auto _bwd_shift(iter_type iter, iter_end_type iter_end, usize steps) const -> void
        {
            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
            {
                std::shift_left(iter.get_data(), iter_end.get_data(), steps);
                return;
            }

            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::shift_left(std_iter, std_iter_end, steps);
        }

        template <class iter_type, class iter_end_type>
        constexpr auto _bwd_rotate(iter_type iter, iter_end_type iter_end, usize steps) const
            -> void
        {
            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
            {
                std::rotate(iter.get_data(), iter.get_data() + steps, iter_end.get_data());
                return;
            }

            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::rotate(std_iter, std_iter + steps, std_iter_end);
        }

        template <class iter_type, class iter_end_type>
        constexpr auto _fwd_rotate(iter_type iter, iter_end_type iter_end, isize steps) const
            -> void
        {
            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
            {
                std::rotate(iter.get_data(), iter_end.get_data() - steps, iter_end.get_data());
                return;
            }

            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::rotate(std_iter, std_iter_end - steps, std_iter_end);
        }

        template <class iter_type, class iter_end_type>
        constexpr auto _destruct(iter_type iter, iter_end_type iter_end) const -> void
        {
            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
            {
                std::destroy(iter.get_data(), iter_end.get_data());
                return;
            }

            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::destroy(std_iter, std_iter_end);
        }
    };
}
