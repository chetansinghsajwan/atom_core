#pragma once

namespace atom
{
    class range_helper
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
            requires rrange<range_type>
        constexpr auto can_get_count() const -> bool
        {
            return rfwd_range<range_type>;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
            requires rfwd_range<range_type>
        constexpr auto get_count(const range_type& range) const -> usize
        {
            return _get_count(range.iter(), range.iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type, class type>
            requires rmut_range<range_type> and rassignable<typename range_type::elem_type, type>
        constexpr auto fill(range_type&& range, type&& val) const -> void
        {
            _fill(range.iter(), range.iter_end(), forward<type>(val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        ATOM_PRAGMA_OPTIMIZE_OFF

        template <class range_type, typename type>
            requires rmut_range<range_type> and rassignable<typename range_type::elem_type, type>
        constexpr auto fill_explicit(range_type&& range, type&& val) const -> void
        {
            _fill(range.iter(), range.iter_end(), forward<type>(val));
        }

        ATOM_PRAGMA_OPTIMIZE_ON

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class trange1, class trange2>
            requires rrange<trange1> and rmut_range<trange2>
                     and rassignable<typename trange2::elem_type, typename trange1::elem_type>
        constexpr auto fwd_copy_to(const trange1& range1, trange2&& range2) const -> void
        {
            _fwd_copy(range1.iter(), range1.iter_end(), range2.iter(), range2.iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class trange1, class trange2>
            requires rrange<trange1> and rmut_range<trange2>
                     and rassignable<typename trange2::elem_type, typename trange1::elem_type>
        constexpr auto bwd_copy_to(const trange1& range1, trange2&& range2) const -> void
        {
            _bwd_copy(range1.iter(), range1.iter_end(), range2.iter(), range2.iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class trange1, class trange2>
            requires rmut_range<trange1> and rmut_range<trange2>
                     and rassignable<typename trange2::elem_type, typename trange1::elem_type>
        constexpr auto fwd_move_to(trange1&& range1, trange2&& range2) const -> void
        {
            _fwd_move(range1.iter(), range1.iter_end(), range2.iter(), range2.iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class trange1, class trange2>
            requires rmut_range<trange1> and rmut_range<trange2>
                     and rassignable<typename trange2::elem_type, typename trange1::elem_type>
        constexpr auto bwd_move_to(trange1&& range1, trange2&& range2) const -> void
        {
            _bwd_move(range1.iter(), range1.iter_end(), range2.iter(), range2.iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
            requires rmut_fwd_range<range_type> and rmove_assignable<typename range_type::elem_type>
        constexpr auto shift_fwd(range_type&& range, usize steps) const -> void
        {
            _fwd_shift(range.iter(), range.iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
            requires rmut_fwd_range<range_type> and rmove_assignable<typename range_type::elem_type>
        constexpr auto shift_bwd(range_type&& range, usize steps) const -> void
        {
            _bwd_shift(range.iter(), range.iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
            requires rmut_fwd_range<range_type> and rswappable<typename range_type::elem_type>
        constexpr auto shift_by(range_type&& range, isize steps) const -> void
        {
            if (steps > 0)
            {
                _bwd_shift(range.iter(), range.iter_end(), steps);
            }
            else
            {
                _fwd_shift(range.iter(), range.iter_end(), -steps);
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
            requires rmut_range<range_type> and rswappable<typename range_type::elem_type>
        constexpr auto rotate_fwd(range_type&& range, usize steps) const -> void
        {
            _fwd_rotate(range.iter(), range.iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
            requires rmut_range<range_type> and rswappable<typename range_type::elem_type>
        constexpr auto rotate_bwd(range_type&& range, usize steps) const -> void
        {
            _bwd_rotate(range.iter(), range.iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
            requires rmut_range<range_type> and rswappable<typename range_type::elem_type>
        constexpr auto rotate_by(range_type&& range, isize steps) const -> void
        {
            if (steps > 0)
            {
                _bwd_rotate(range.iter(), range.iter_end(), steps);
            }
            else
            {
                _fwd_rotate(range.iter(), range.iter_end(), -steps);
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_type>
            requires rmut_range<range_type> and rdestructible<typename range_type::elem_type>
        constexpr auto destruct(range_type&& range) const -> void
        {
            _destruct(range.iter(), range.iter_end());
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
            for (; not iter.eq(iter_end); iter.next())
                count++;

            return count;
        }

        template <class iter_type, class iter_end_type, typename type>
        constexpr auto _fill(iter_type iter, iter_end_type iter_end, type&& val) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter{ iter };
            std_iter_wrap_for_atom_iter std_iter_end{ iter_end };

            std::fill(std_iter, std_iter_end, forward<type>(val));
        }

        template <class titer1, class titer_end1, class titer2, class titer_end2>
        constexpr auto _fwd_copy(
            titer1 iter1, titer_end1 iter_end1, titer2 iter2, titer_end2 iter_end2) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter1{ iter1 };
            std_iter_wrap_for_atom_iter std_iter_end1{ iter_end1 };
            std_iter_wrap_for_atom_iter std_iter2{ iter2 };

            std::copy(std_iter1, std_iter_end1, std_iter2);
        }

        template <class titer1, class titer_end1, class titer2, class titer_end2>
        constexpr auto _bwd_copy(
            titer1 iter1, titer_end1 iter_end1, titer2 iter2, titer_end2 iter_end2) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter1{ iter1 };
            std_iter_wrap_for_atom_iter std_iter_end1{ iter_end1 };
            std_iter_wrap_for_atom_iter std_iter2{ iter2 };

            std::copy_backward(std_iter1, std_iter_end1, std_iter2);
        }

        template <class titer1, class titer_end1, class titer2, class titer_end2>
        constexpr auto _fwd_move(
            titer1 iter1, titer_end1 iter_end1, titer2 iter2, titer_end2 iter_end2) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter1{ iter1 };
            std_iter_wrap_for_atom_iter std_iter_end1{ iter_end1 };
            std_iter_wrap_for_atom_iter std_iter2{ iter2 };

            std::move(std_iter1, std_iter_end1, std_iter2);
        }

        template <class titer1, class titer_end1, class titer2, class titer_end2>
        constexpr auto _bwd_move(
            titer1 iter1, titer_end1 iter_end1, titer2 iter2, titer_end2 iter_end2) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter1{ iter1 };
            std_iter_wrap_for_atom_iter std_iter_end1{ iter_end1 };
            std_iter_wrap_for_atom_iter std_iter2{ iter2 };

            std::move_backward(std_iter1, std_iter_end1, std_iter2);
        }

        template <class iter_type, class iter_end_type>
        constexpr auto _fwd_shift(iter_type iter, iter_end_type iter_end, usize steps) const -> void
        {
            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
            {
                std::shift_right(iter.data(), iter_end.data(), steps);
                return;
            }

            std_iter_wrap_for_atom_iter std_iter{ iter };
            std_iter_wrap_for_atom_iter std_iter_end{ iter_end };

            std::shift_right(std_iter, std_iter_end, steps);
        }

        template <class iter_type, class iter_end_type>
        constexpr auto _bwd_shift(iter_type iter, iter_end_type iter_end, usize steps) const -> void
        {
            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
            {
                std::shift_left(iter.data(), iter_end.data(), steps);
                return;
            }

            std_iter_wrap_for_atom_iter std_iter{ iter };
            std_iter_wrap_for_atom_iter std_iter_end{ iter_end };

            std::shift_left(std_iter, std_iter_end, steps);
        }

        template <class iter_type, class iter_end_type>
        constexpr auto _bwd_rotate(iter_type iter, iter_end_type iter_end, usize steps) const
            -> void
        {
            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
            {
                std::rotate(iter.data(), iter.data() + steps, iter_end.data());
                return;
            }

            std_iter_wrap_for_atom_iter std_iter{ iter };
            std_iter_wrap_for_atom_iter std_iter_end{ iter_end };

            std::rotate(std_iter, std_iter + steps, std_iter_end);
        }

        template <class iter_type, class iter_end_type>
        constexpr auto _fwd_rotate(iter_type iter, iter_end_type iter_end, isize steps) const
            -> void
        {
            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
            {
                std::rotate(iter.data(), iter_end.data() - steps, iter_end.data());
                return;
            }

            std_iter_wrap_for_atom_iter std_iter{ iter };
            std_iter_wrap_for_atom_iter std_iter_end{ iter_end };

            std::rotate(std_iter, std_iter_end - steps, std_iter_end);
        }

        template <class iter_type, class iter_end_type>
        constexpr auto _destruct(iter_type iter, iter_end_type iter_end) const -> void
        {
            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
            {
                std::destroy(iter.data(), iter_end.data());
                return;
            }

            std_iter_wrap_for_atom_iter std_iter{ iter };
            std_iter_wrap_for_atom_iter std_iter_end{ iter_end };

            std::destroy(std_iter, std_iter_end);
        }
    };
}
