#include "catch2/catch_test_macros.hpp"

import atom.core;

using namespace atom;

TEST_CASE("atom.core.bitset")
{
    bitset8 bits = _u8(0b10101010);
    bitset8 bits0 = _u8(0b00000000);
    bitset8 bits1 = _u8(0b11111111);

    REQUIRE(bits.get_at(0) == false);
    REQUIRE(bits.get_at(1) == true);

    bits.set_at(0, true);
    REQUIRE(bits.get_at(0) == true);
    bits.flip_at(0);
    REQUIRE(bits.get_at(0) == false);

    REQUIRE(bits.count_ones() == 4);
    REQUIRE(bits.count_zeros() == 4);

    REQUIRE(bits.count_leading_ones() == 1);
    REQUIRE(bits.count_leading_zeros() == 0);
    REQUIRE(bits.count_trailing_ones() == 0);
    REQUIRE(bits.count_trailing_zeros() == 1);
    REQUIRE(bits.find_leading_one() == 7);
    REQUIRE(bits.find_leading_zero() == 6);
    REQUIRE(bits.find_trailing_one() == 1);
    REQUIRE(bits.find_trailing_zero() == 0);

    REQUIRE(bits.are_all_one() == false);
    REQUIRE(bits1.are_all_one() == true);
    REQUIRE(bits.are_all_zero() == false);
    REQUIRE(bits0.are_all_zero() == true);
    REQUIRE(bits.is_any_one() == true);
    REQUIRE(bits.is_any_zero() == true);
    REQUIRE(bits.is_only_one() == true);
    REQUIRE(bits.is_only_zero() == true);

    bits.flip_all();
    REQUIRE(bits == _u8(0b01010101));

    REQUIRE(bits.shift_left(1) == _u8(0b01010100));
    REQUIRE(bits.shift_right(1) == _u8(0b00101010));
    REQUIRE(bits.shift_by(-1) == _u8(0b01010100));
    REQUIRE(bits.shift_by(1) == _u8(0b00101010));

    REQUIRE(bits.rotate_left(3) == _u8(0b01010001));
    REQUIRE(bits.rotate_right(1) == _u8(0b10101000));
    REQUIRE(bits.rotate_by(-1) == _u8(0b01010001));
    REQUIRE(bits.rotate_by(1) == _u8(0b10101000));

    REQUIRE(bits.is_eq(bits));
}
