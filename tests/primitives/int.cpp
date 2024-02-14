import atom.core;
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_template_test_macros.hpp"

using namespace atom;

struct i32_test_data
{
    using int_type = i32;
    using small_int_type = i16;
    using big_int_type = i64;

    static constexpr _i64 INT_BITS = 32;
    static constexpr _i64 INT_MIN = -2'147'483'648;
    static constexpr _i64 INT_MAX = 2'147'483'647;
    static constexpr _i64 INT_SMALL_MIN = -32'768;
    static constexpr _i64 INT_SMALL_MAX = 32'767;
};

struct u32_test_data
{
    using int_type = u32;
    using small_int_type = u16;
    using big_int_type = u64;

    static constexpr _u64 INT_BITS = 32;
    static constexpr _u64 INT_MIN = 0;
    static constexpr _u64 INT_MAX = 4'294'967'295;
    static constexpr _u64 INT_SMALL_MIN = 0;
    static constexpr _u64 INT_SMALL_MAX = 65'535;
};

TEMPLATE_TEST_CASE("atom.core.integers", "", i32_test_data, u32_test_data)
{
    using int_type = TestType::int_type;
    using small_int_type = TestType::small_int_type;
    using unwrapped_small_int_type = small_int_type::unwrapped_type;
    using big_int_type = TestType::big_int_type;
    constexpr auto INT_BITS = TestType::INT_BITS;
    constexpr auto INT_MIN = TestType::INT_MIN;
    constexpr auto INT_MAX = TestType::INT_MAX;
    constexpr auto INT_SMALL_MIN = TestType::INT_SMALL_MIN;
    constexpr auto INT_SMALL_MAX = TestType::INT_SMALL_MAX;

    SECTION("from conversions")
    {
        REQUIRE(int_type::is_conversion_safe_from(big_int_type(INT_MIN)));
        REQUIRE(int_type::is_conversion_safe_from(big_int_type(INT_MAX)));

        REQUIRE_FALSE(int_type::is_conversion_safe_from(big_int_type(INT_MIN - 1)));
        REQUIRE_FALSE(int_type::is_conversion_safe_from(big_int_type(INT_MAX + 1)));

        REQUIRE_THROWS_AS(
            int_type::from_checked(big_int_type(INT_MAX + 1)), contract_violation_exception);
        REQUIRE(int_type::from_unchecked(big_int_type(INT_MAX + 1)).to_unwrapped() ==
                typename int_type::unwrapped_type(INT_MAX + 1));
    }

    SECTION("to conversions")
    {
        REQUIRE(int_type(INT_SMALL_MAX).template is_conversion_safe_to<small_int_type>());

        REQUIRE_FALSE(int_type(INT_SMALL_MAX + 1).template is_conversion_safe_to<small_int_type>());

        REQUIRE(int_type(INT_SMALL_MAX).template to_checked<small_int_type>() == INT_SMALL_MAX);

        REQUIRE_THROWS_AS(int_type(INT_SMALL_MAX + 1).template to_checked<small_int_type>(),
            contract_violation_exception);

        REQUIRE(int_type(INT_SMALL_MAX + 1).template to_unchecked<small_int_type>()
                == small_int_type(INT_SMALL_MAX + 1));
    }

    SECTION("from_unwrapped conversions")
    {
        REQUIRE(int_type::is_conversion_safe_from_unwrapped(INT_MIN));
        REQUIRE(int_type::is_conversion_safe_from_unwrapped(INT_MAX));
        REQUIRE_FALSE(int_type::is_conversion_safe_from_unwrapped(INT_MIN - 1));
        REQUIRE_FALSE(int_type::is_conversion_safe_from_unwrapped(INT_MAX + 1));

        REQUIRE_THROWS_AS(
            int_type::from_unwrapped_checked(INT_MAX + 1), contract_violation_exception);
        REQUIRE(int_type::from_unwrapped_unchecked(INT_MAX + 1).to_unwrapped() ==
                typename int_type::unwrapped_type(INT_MAX + 1));
    }

    SECTION("to_unwrapped conversions")
    {
        REQUIRE(int_type(INT_SMALL_MAX)
                    .template is_conversion_safe_to_unwrapped<unwrapped_small_int_type>());

        REQUIRE_FALSE(int_type(INT_SMALL_MAX + 1)
                          .template is_conversion_safe_to_unwrapped<unwrapped_small_int_type>());

        REQUIRE(int_type(INT_SMALL_MAX).template to_unwrapped_checked<unwrapped_small_int_type>()
                == INT_SMALL_MAX);

        REQUIRE_THROWS_AS(
            int_type(INT_SMALL_MAX + 1).template to_unwrapped_checked<unwrapped_small_int_type>(),
            contract_violation_exception);

        REQUIRE(
            int_type(INT_SMALL_MAX + 1).template to_unwrapped_unchecked<unwrapped_small_int_type>()
            == unwrapped_small_int_type(INT_SMALL_MAX + 1));
    }

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
        REQUIRE(int_type::min() == INT_MIN);
        REQUIRE(int_type::max() == INT_MAX);
        REQUIRE(int_type::bits() == INT_BITS);

        REQUIRE(int_type(10).min_of(20) == 10);
        REQUIRE(int_type(10).max_of(20) == 20);
        REQUIRE(int_type(5).clamp(10, 20) == 10);
        REQUIRE(int_type(15).clamp(10, 20) == 15);
        REQUIRE(int_type(25).clamp(10, 20) == 20);

        if constexpr (int_type::is_signed())
        {
            REQUIRE(int_type(1).clamp(-5, 7) == 1);
            REQUIRE(int_type(-7).clamp(-5, 7) == -5);
        }

        REQUIRE(int_type(100).count_digits() == 3);

        if constexpr (int_type::is_signed())
        {
            REQUIRE(int_type(-100).count_digits() == 3);
        }

        SECTION("pow()")
        {
            REQUIRE(int_type(2).is_pow_safe(5));
            REQUIRE_FALSE(int_type::max().is_pow_safe(2));

            REQUIRE(int_type(2).pow_checked(5) == 32);
            REQUIRE_THROWS_AS(int_type::max().pow_checked(2), contract_violation_exception);

            REQUIRE(int_type(2).is_pow2_safe());
            REQUIRE_FALSE(int_type::max().is_pow2_safe());

            REQUIRE(int_type(2).pow2_checked() == 4);
            REQUIRE_THROWS_AS(int_type::max().pow2_checked(), contract_violation_exception);

            REQUIRE(int_type(2).is_pow3_safe());
            REQUIRE_FALSE(int_type::max().is_pow3_safe());

            REQUIRE(int_type(2).pow3_checked() == 8);
            REQUIRE_THROWS_AS(int_type::max().pow3_checked(), contract_violation_exception);

            if constexpr (int_type::is_signed())
            {
                REQUIRE(int_type(-2).is_pow_safe(5));
                REQUIRE_FALSE(int_type::max().is_pow_safe(2));

                REQUIRE(int_type(-2).pow_checked(5) == -32);
                REQUIRE_THROWS_AS(int_type::max().pow_checked(2), contract_violation_exception);

                REQUIRE(int_type(-2).is_pow2_safe());
                REQUIRE_FALSE(int_type::max().is_pow2_safe());

                REQUIRE(int_type(-2).pow2_checked() == 4);
                REQUIRE_THROWS_AS(int_type::max().pow2_checked(), contract_violation_exception);

                REQUIRE(int_type(-2).is_pow3_safe());
                REQUIRE_FALSE(int_type::max().is_pow3_safe());

                REQUIRE(int_type(-2).pow3_checked() == -8);
                REQUIRE_THROWS_AS(int_type::max().pow3_checked(), contract_violation_exception);
            }
        }

        SECTION("root()")
        {
            REQUIRE(int_type(2).is_root_safe(5));
            REQUIRE_FALSE(int_type(2).is_root_safe(0));

            if constexpr (int_type::is_signed())
            {
                REQUIRE(int_type(-45).is_root_safe(3));
                REQUIRE_FALSE(int_type(-45).is_root_safe(2));
            }

            REQUIRE(int_type(32).root_checked(5) == 2);
            REQUIRE(int_type(2).is_root2_safe());

            if constexpr (int_type::is_signed())
            {
                REQUIRE_FALSE(int_type(-23).is_root2_safe());
            }

            REQUIRE(int_type(4).root2_checked() == 2);
            REQUIRE(int_type(8).root3_checked() == 2);
        }

        SECTION("log()")
        {
            REQUIRE(int_type(2).is_log_safe(3));
            REQUIRE_FALSE(int_type(0).is_log_safe(2));
            REQUIRE_FALSE(int_type(2).is_log_safe(0));
            REQUIRE_FALSE(int_type(2).is_log_safe(1));

            if constexpr (int_type::is_signed())
            {
                REQUIRE_FALSE(int_type(-1).is_log_safe(2));
            }

            REQUIRE(int_type(19).log_checked(3) == 2);

            REQUIRE(int_type(2).is_log2_safe());
            REQUIRE_FALSE(int_type(0).is_log2_safe());
            REQUIRE(int_type(2).log2_checked() == 1);

            REQUIRE(int_type(10).is_log10_safe());
            REQUIRE_FALSE(int_type(0).is_log10_safe());
            REQUIRE(int_type(10).log10_checked() == 1);
        }

        if constexpr (int_type::is_signed())
        {
            REQUIRE_FALSE(int_type(-1).is_pos());
            REQUIRE(int_type(+1).is_pos());
            REQUIRE(int_type(-1).is_neg());
            REQUIRE_FALSE(int_type(+1).is_neg());

            REQUIRE(int_type(-1).abs_checked() == 1);
            REQUIRE(int_type(1).abs_checked() == 1);

            REQUIRE(int_type(-1).is_abs_safe());
            REQUIRE_FALSE(int_type::min().is_abs_safe());

            REQUIRE_THROWS_AS(int_type::min().abs_checked(), contract_violation_exception);
            REQUIRE_NOTHROW(int_type::min().abs_unchecked());

            REQUIRE(int_type(-1).neg_checked() == 1);
            REQUIRE(int_type(1).neg_checked() == -1);

            REQUIRE(int_type(-1).is_neg_safe());
            REQUIRE_FALSE(int_type::min().is_neg_safe());

            REQUIRE_THROWS_AS(int_type::min().neg_checked(), contract_violation_exception);
            REQUIRE_NOTHROW(int_type::min().neg_unchecked());

            REQUIRE(int_type(INT_MIN).sign() == -1);
            REQUIRE(int_type(INT_MAX).sign() == 1);
        }
    }
}
