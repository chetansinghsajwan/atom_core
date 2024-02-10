import atom.core;
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_template_test_macros.hpp"

using namespace atom;

TEST_CASE("atom.core.unsigned_integer")
{
    constexpr auto U32_MIN = 0;
    constexpr auto U32_MAX = 4'294'967'295;
    constexpr auto U16_MIN = 0;
    constexpr auto U16_MAX = 65'535;

    REQUIRE(u32::min() == U32_MIN);
    REQUIRE(u32::max() == U32_MAX);

    // SECTION("from conversions")
    // {
    //     REQUIRE(u32::is_conversion_safe_from(u64(U32_MIN)));
    //     REQUIRE(u32::is_conversion_safe_from(u64(U32_MAX)));
    //     REQUIRE_FALSE(u32::is_conversion_safe_from(u64(U32_MIN - 1)));
    //     REQUIRE_FALSE(u32::is_conversion_safe_from(u64(U32_MAX + 1)));

    //     REQUIRE_THROWS_AS(u32::from_checked(U32_MIN - 1), contract_violation_exception);
    //     REQUIRE(
    //         u32::from_unwrapped_unchecked(U32_MIN - 1).to_unwrapped() == u32::unwrapped_type(U32_MIN - 1));
    // }

    // SECTION("to conversions")
    // {
    //     REQUIRE(u32(U16_MAX).is_conversion_safe_to<u16>());
    //     REQUIRE_FALSE(u32(U16_MAX + 1).is_conversion_safe_to<u16>());

    //     REQUIRE(u32(U16_MAX).to_checked<u16>() == U16_MAX);
    //     REQUIRE_THROWS_AS(u32(U16_MAX + 1).to_checked<u16>(), contract_violation_exception);
    //     REQUIRE(u32(U16_MAX + 1).to_unchecked<u16>() == u16::unwrapped_type(U16_MAX + 1));
    // }

    SECTION("form_unwrapped conversions")
    {
        REQUIRE(u32::is_conversion_safe_from_unwrapped(U32_MIN));
        REQUIRE(u32::is_conversion_safe_from_unwrapped(U32_MAX));
        REQUIRE_FALSE(u32::is_conversion_safe_from_unwrapped(-1));
        REQUIRE_FALSE(u32::is_conversion_safe_from_unwrapped(_u64(U32_MAX) + 1));

        REQUIRE_THROWS_AS(u32::from_unwrapped_checked(U32_MIN - 1), contract_violation_exception);
        REQUIRE(
            u32::from_unwrapped_unchecked(U32_MIN - 1).to_unwrapped() == u32::unwrapped_type(U32_MIN - 1));
    }

    SECTION("to_unwrapped conversions")
    {
        REQUIRE(u32(U16_MAX).is_conversion_safe_to_unwrapped<uint16_t>());
        REQUIRE_FALSE(u32(U16_MAX + 1).is_conversion_safe_to_unwrapped<uint16_t>());

        REQUIRE(u32(U16_MAX).to_unwrapped_checked<uint16_t>() == U16_MAX);
        REQUIRE_THROWS_AS(
            u32(U16_MAX + 1).to_unwrapped_checked<uint16_t>(), contract_violation_exception);
        REQUIRE(u32(U16_MAX + 1).to_unwrapped_unchecked<uint16_t>() == uint16_t(U16_MAX + 1));
    }

    SECTION("utils")
    {
        REQUIRE(u32(100).count_digits() == 3);

        REQUIRE(u32(2).is_pow_safe(5));
        REQUIRE_FALSE(u32::max().is_pow_safe(2));

        REQUIRE(u32(2).pow_checked(5) == 32);
        REQUIRE_THROWS_AS(u32::max().pow_checked(2), contract_violation_exception);

        REQUIRE(u32(16).root(4) == 2);
        REQUIRE(u32(10).root2() == 3);
        REQUIRE(u32(8).root3() == 2);

        REQUIRE(u32(5).log(5) == 1);
        REQUIRE(u32(2).log2() == 1);
        REQUIRE(u32(10).log10() == 1);
    }
}

TEMPLATE_TEST_CASE(
    "atom.core.int", "[primitives]", i8, i16, i32, i64, isize, imax, u8, u16, u32, u64, usize, umax)
{
    using int_type = TestType;

    SECTION("addition")
    {
        int_type n = 0;

        REQUIRE(n.add_checked(1) == 1);
        REQUIRE(n == 0);

        n.add_assign_checked(1);
        REQUIRE(n == 1);

        REQUIRE_THROWS_AS(int_type::max().add_checked(1), contract_violation_exception);

        REQUIRE(int_type::max().add_unchecked(1).to_unwrapped() ==
                typename int_type::unwrapped_type(int_type::max().to_unwrapped() + 1));

        REQUIRE(int_type::max().is_add_safe(0));
        REQUIRE(int_type(0).is_add_safe(int_type::max()));
        REQUIRE_FALSE(int_type::max().is_add_safe(1));
    }

    SECTION("subtraction")
    {
        int_type n = 1;

        REQUIRE(n.sub_checked(1) == 0);
        REQUIRE(n == 1);

        n.sub_assign_checked(1);
        REQUIRE(n == 0);

        REQUIRE_THROWS_AS(int_type::min().sub_checked(1), contract_violation_exception);
        REQUIRE(int_type::min().sub_unchecked(1).to_unwrapped() ==
                typename int_type::unwrapped_type(int_type::min().to_unwrapped() - 1));

        REQUIRE(int_type::min().is_sub_safe(0));
        REQUIRE(int_type(0).is_sub_safe(int_type::min()));
        REQUIRE_FALSE(int_type::min().is_sub_safe(1));
    }

    SECTION("multiplication")
    {
        int_type n = 1;

        REQUIRE(n.mul_checked(2) == 2);
        REQUIRE(n == 1);

        n.mul_assign_checked(2);
        REQUIRE(n == 2);

        REQUIRE_THROWS_AS(int_type::max().mul_checked(2), contract_violation_exception);
        REQUIRE(int_type::max().mul_unchecked(2).to_unwrapped() ==
                typename int_type::unwrapped_type(int_type::max().to_unwrapped() * 2));

        REQUIRE(int_type::max().is_mul_safe(1));
        REQUIRE(int_type(1).is_mul_safe(int_type::max()));
        REQUIRE_FALSE(int_type::max().is_mul_safe(2));
    }

    SECTION("division")
    {
        SECTION("quotient")
        {
            int_type n = 5;

            REQUIRE(n.div_checked(2) == 2);
            REQUIRE(n == 5);

            n.div_assign_checked(2);
            REQUIRE(n == 2);
        }

        SECTION("remainder")
        {
            int_type n = 5;

            REQUIRE(n.rem_checked(2) == 1);
            REQUIRE(n == 5);

            n.rem_assign_checked(2);
            REQUIRE(n == 1);
        }

        if constexpr (int_type::is_signed())
        {
            REQUIRE_THROWS_AS(int_type::min().div_checked(-1), contract_violation_exception);
            REQUIRE_FALSE(int_type::min().is_div_safe(-1));
        }
    }

    SECTION("utils")
    {
        REQUIRE(int_type(10).min_of(20) == 10);
        REQUIRE(int_type(10).max_of(20) == 20);
        REQUIRE(int_type(5).clamp(10, 20) == 10);
        REQUIRE(int_type(15).clamp(10, 20) == 15);
        REQUIRE(int_type(25).clamp(10, 20) == 20);
    }
}
