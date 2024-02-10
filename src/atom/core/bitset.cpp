export module atom.core:core.bitset;
import :core.int_wrapper;
import :contracts_decl;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// implmentation for `bitset`.
    /// --------------------------------------------------------------------------------------------
    export template <usize count>
    class _bitset_impl;

    /// --------------------------------------------------------------------------------------------
    /// type to hold and manage a number of bits.
    /// --------------------------------------------------------------------------------------------
    export template <usize count>
    class bitset
    {
        using this_type = bitset<count>;
        using _impl_type = _bitset_impl<count>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr bitset() = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr bitset(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr bitset& operator=(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------

        constexpr bitset(this_type&& that) = default;
        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr bitset& operator=(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~bitset() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        /// gets the bit at index `i`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_at(usize i) const -> bool
        {
            return _impl.get_at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the bit at index `i`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto set_at(usize i, bool bit)
        {
            _impl.set_at(i, bit);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the count of bits set to `1`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count_ones() const -> usize
        {
            return _impl.count_ones();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the count of bits set to `0`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count_zeros() const -> usize
        {
            return _impl.count_zeros();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the count of continuous bits set to `0` from the left.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count_leading_ones() const -> usize
        {
            return _impl.count_leading_ones();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the count of continuous bits set to `0` from the left.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count_leading_zeros() const -> usize
        {
            return _impl.count_leading_zeros();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the count of continuous bits set to `0` from the right.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count_trailing_ones() const -> usize
        {
            return _impl.count_trailing_ones();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the count of continuous bits set to `0` from the right.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count_trailing_zeros() const -> usize
        {
            return _impl.count_trailing_zeros();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the position of first bit set to `1` from the left.
        /// ----------------------------------------------------------------------------------------
        constexpr auto find_leading_one() const -> usize
        {
            return _impl.find_leading_one();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the position of first bit set to `0` from the left.
        /// ----------------------------------------------------------------------------------------
        constexpr auto find_leading_zero() const -> usize
        {
            return _impl.find_leading_zero();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the position of first bit set to `1` from the right.
        /// ----------------------------------------------------------------------------------------
        constexpr auto find_trailing_one() const -> usize
        {
            return _impl.find_trailing_one();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the position of first bit set to `0` from the right.
        /// ----------------------------------------------------------------------------------------
        constexpr auto find_trailing_zero() const -> usize
        {
            return _impl.find_trailing_zero();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if all bits are `1`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto are_all_one() const -> bool
        {
            return _impl.are_all_one();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if all bits are `0`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto are_all_zero() const -> bool
        {
            return _impl.are_all_zero();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if any bit is `1`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_any_one() const -> bool
        {
            return _impl.is_any_one();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if any bit is `0`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_any_zero() const -> bool
        {
            return _impl.is_any_zero();
        }

        /// ----------------------------------------------------------------------------------------
        /// reverses all bits.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reverse()
        {
            _impl.reverse();
        }

        /// ----------------------------------------------------------------------------------------
        /// retuns result after shifting bits left by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_left(usize shifts) -> this_type&
        {
            contracts::debug_expects(is_shift_left_safe(shifts));

            _impl.shift_left(shifts);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// retuns result after shifting bits left by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_left_checked(usize shifts) -> this_type&
        {
            contracts::expects(is_shift_left_safe(shifts));

            _impl.shift_left(shifts);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// retuns result after shifting bits left by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_left_unchecked(usize shifts) -> this_type&
        {
            _impl.shift_left(shifts);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no data loss in left shift operation.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_shift_left_safe(usize shifts) const -> bool
        {
            _impl.is_shift_left_safe(shifts);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// retuns result after shifting bits right by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_right(usize shifts) -> this_type&
        {
            contracts::debug_expects(is_shift_right_safe(shifts));

            _impl.shift_right(shifts);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// retuns result after shifting bits right by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_right_checked(usize shifts) -> this_type&
        {
            contracts::expects(is_shift_right_safe(shifts));

            _impl.shift_right(shifts);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// retuns result after shifting bits right by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_right_unchecked(usize shifts) -> this_type&
        {
            _impl.shift_right(shifts);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no data loss in right shift operation.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_shift_right_safe(usize shifts) const -> bool
        {
            return _impl.is_shift_right_safe(shifts);
        }

        /// ----------------------------------------------------------------------------------------
        /// retuns result after shifting bits by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_by(isize shifts) -> this_type&
        {
            contracts::debug_expects(is_shift_by_safe(shifts));

            return _impl.shift_by(shifts);
        }

        /// ----------------------------------------------------------------------------------------
        /// retuns result after shifting bits by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_by_checked(isize shifts) -> this_type&
        {
            contracts::expects(is_shift_by_safe(shifts));

            return _impl.shift_by(shifts);
        }

        /// ----------------------------------------------------------------------------------------
        /// retuns result after shifting bits by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_by_unchecked(isize shifts) -> this_type&
        {
            return _impl.shift_by(shifts);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no data loss in left or right shift operation.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_shift_by_safe(isize shifts) const -> bool
        {
            return _impl.is_shift_by_safe(shifts);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after rotating bits left by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_left(usize shifts) -> this_type&
        {
            return _impl.rotate_left(shifts);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after rotating bits right by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_right(usize shifts) -> this_type&
        {
            return _impl.rotate_right(shifts);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after rotating bits by `shifts`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_by(isize shifts) -> this_type&
        {
            return _impl.rotate_by(shifts);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if values of all bits matches value of `that` bits.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_eq(const this_type& that) const -> bool
        {
            return _impl.is_eq(that._impl);
        }

    public:
        _impl_type _impl;
    };

    export using bitset8 = bitset<8>;
    export using bitset16 = bitset<16>;
    export using bitset32 = bitset<32>;
    export using bitset64 = bitset<64>;
    export using bitset128 = bitset<128>;
}
