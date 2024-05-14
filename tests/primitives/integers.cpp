// import atom.core;
// #include "catch2/catch_test_macros.hpp"
// #include "catch2/catch_template_test_macros.hpp"

// using namespace atom;

// struct i32_test_data
// {
//     using int_t = i32;
//     using small_int_t = i16;
//     using big_int_t = i64;

//     static constexpr i64 int_bits = 32;
//     static constexpr i64 int_min = -2'147'483'648;
//     static constexpr i64 int_max = 2'147'483'647;
//     static constexpr i64 small_int_min = -32'768;
//     static constexpr i64 small_int_max = 32'767;
// };

// struct u32_test_data
// {
//     using int_t = u32;
//     using small_int_t = u16;
//     using big_int_t = u64;

//     static constexpr u64 int_bits = 32;
//     static constexpr u64 int_min = 0;
//     static constexpr u64 int_max = 4'294'967'295;
//     static constexpr u64 small_int_min = 0;
//     static constexpr u64 small_int_max = 65'535;
// };

// TEMPLATE_TEST_CASE("atom.core.integers", "", i32_test_data, u32_test_data)
// {
//     using int_t = TestType::int_t;
//     using small_int_t = TestType::small_int_t;
//     using unwrapped_small_int_t = small_int_t::unwrapped_t;
//     using big_int_t = TestType::big_int_t;
//     constexpr auto int_bits = TestType::int_bits;
//     constexpr auto int_min = TestType::int_min;
//     constexpr auto int_max = TestType::int_max;
//     constexpr auto small_int_min = TestType::small_int_min;
//     constexpr auto small_int_max = TestType::small_int_max;

//     SECTION("from conversions")
//     {
//         REQUIRE(int_t::is_conversion_safe_from(big_int_t(int_min)));
//         REQUIRE(int_t::is_conversion_safe_from(big_int_t(int_max)));

//         REQUIRE_FALSE(int_t::is_conversion_safe_from(big_int_t(int_min - 1)));
//         REQUIRE_FALSE(int_t::is_conversion_safe_from(big_int_t(int_max + 1)));

//         REQUIRE_THROWS_AS(
//             int_t::from_checked(big_int_t(int_max + 1)), contract_violation_exception);
//         REQUIRE(int_t::from_unchecked(big_int_t(int_max + 1)) ==
//                 typename int_t::unwrapped_t(int_max + 1));
//     }

//     SECTION("to conversions")
//     {
//         REQUIRE(int_t(small_int_max).template is_conversion_safe_to<small_int_t>());

//         REQUIRE_FALSE(int_t(small_int_max + 1).template is_conversion_safe_to<small_int_t>());

//         REQUIRE(int_t(small_int_max).template to_checked<small_int_t>() == small_int_max);

//         REQUIRE_THROWS_AS(int_t(small_int_max + 1).template to_checked<small_int_t>(),
//             contract_violation_exception);

//         REQUIRE(int_t(small_int_max + 1).template to_unchecked<small_int_t>()
//                 == small_int_t(small_int_max + 1));
//     }

//     SECTION("from_unwrapped conversions")
//     {
//         REQUIRE(int_t::is_conversion_safe_from_unwrapped(int_min));
//         REQUIRE(int_t::is_conversion_safe_from_unwrapped(int_max));
//         REQUIRE_FALSE(int_t::is_conversion_safe_from_unwrapped(int_min - 1));
//         REQUIRE_FALSE(int_t::is_conversion_safe_from_unwrapped(int_max + 1));

//         REQUIRE_THROWS_AS(
//             int_t::from_unwrapped_checked(int_max + 1), contract_violation_exception);
//         REQUIRE(int_t::from_unwrapped_unchecked(int_max + 1) ==
//                 typename int_t::unwrapped_t(int_max + 1));
//     }

//     SECTION("to_unwrapped conversions")
//     {
//         REQUIRE(int_t(small_int_max)
//                     .template is_conversion_safe_to_unwrapped<unwrapped_small_int_t>());

//         REQUIRE_FALSE(int_t(small_int_max + 1)
//                           .template is_conversion_safe_to_unwrapped<unwrapped_small_int_t>());

//         REQUIRE(int_t(small_int_max).template to_unwrapped_checked<unwrapped_small_int_t>()
//                 == small_int_max);

//         REQUIRE_THROWS_AS(
//             int_t(small_int_max + 1).template to_unwrapped_checked<unwrapped_small_int_t>(),
//             contract_violation_exception);

//         REQUIRE(
//             int_t(small_int_max + 1).template to_unwrapped_unchecked<unwrapped_small_int_t>()
//             == unwrapped_small_int_t(small_int_max + 1));
//     }

//     SECTION("addition")
//     {
//         int_t n = 0;

//         REQUIRE(n.add_checked(1) == 1);
//         REQUIRE(n == 0);

//         n.add_assign_checked(1);
//         REQUIRE(n == 1);

//         REQUIRE_THROWS_AS(int_t::max().add_checked(1), contract_violation_exception);

//         REQUIRE(int_t::max().add_unchecked(1) ==
//                 typename int_t::unwrapped_t(int_t::max() + 1));

//         REQUIRE(int_t::max().is_add_safe(0));
//         REQUIRE(int_t(0).is_add_safe(int_t::max()));
//         REQUIRE_FALSE(int_t::max().is_add_safe(1));
//     }

//     SECTION("subtraction")
//     {
//         int_t n = 1;

//         REQUIRE(n.sub_checked(1) == 0);
//         REQUIRE(n == 1);

//         n.sub_assign_checked(1);
//         REQUIRE(n == 0);

//         REQUIRE_THROWS_AS(int_t::min().sub_checked(1), contract_violation_exception);
//         REQUIRE(int_t::min().sub_unchecked(1) ==
//                 typename int_t::unwrapped_t(int_t::min() - 1));

//         REQUIRE(int_t::min().is_sub_safe(0));
//         REQUIRE(int_t(0).is_sub_safe(int_t::min()));
//         REQUIRE_FALSE(int_t::min().is_sub_safe(1));
//     }

//     SECTION("multiplication")
//     {
//         int_t n = 1;

//         REQUIRE(n.mul_checked(2) == 2);
//         REQUIRE(n == 1);

//         n.mul_assign_checked(2);
//         REQUIRE(n == 2);

//         REQUIRE_THROWS_AS(int_t::max().mul_checked(2), contract_violation_exception);
//         REQUIRE(int_t::max().mul_unchecked(2) ==
//                 typename int_t::unwrapped_t(int_t::max() * 2));

//         REQUIRE(int_t::max().is_mul_safe(1));
//         REQUIRE(int_t(1).is_mul_safe(int_t::max()));
//         REQUIRE_FALSE(int_t::max().is_mul_safe(2));
//     }

//     SECTION("division")
//     {
//         SECTION("quotient")
//         {
//             int_t n = 5;

//             REQUIRE(n.div_checked(2) == 2);
//             REQUIRE(n == 5);

//             n.div_assign_checked(2);
//             REQUIRE(n == 2);
//         }

//         SECTION("remainder")
//         {
//             int_t n = 5;

//             REQUIRE(n.rem_checked(2) == 1);
//             REQUIRE(n == 5);

//             n.rem_assign_checked(2);
//             REQUIRE(n == 1);
//         }

//         if constexpr (int_t::is_signed())
//         {
//             REQUIRE_THROWS_AS(int_t::min().div_checked(-1), contract_violation_exception);
//             REQUIRE_FALSE(int_t::min().is_div_safe(-1));
//         }
//     }

//     SECTION("utils")
//     {
//         REQUIRE(int_t::min() == int_min);
//         REQUIRE(int_t::max() == int_max);
//         REQUIRE(int_t::bits() == int_bits);

//         REQUIRE(int_t(10).min_of(20) == 10);
//         REQUIRE(int_t(10).max_of(20) == 20);
//         REQUIRE(int_t(5).clamp(10, 20) == 10);
//         REQUIRE(int_t(15).clamp(10, 20) == 15);
//         REQUIRE(int_t(25).clamp(10, 20) == 20);

//         if constexpr (int_t::is_signed())
//         {
//             REQUIRE(int_t(1).clamp(-5, 7) == 1);
//             REQUIRE(int_t(-7).clamp(-5, 7) == -5);
//         }

//         REQUIRE(int_t(100).count_digits() == 3);

//         if constexpr (int_t::is_signed())
//         {
//             REQUIRE(int_t(-100).count_digits() == 3);
//         }

//         if constexpr (int_t::is_signed())
//         {
//             REQUIRE_FALSE(int_t(-1).is_pos());
//             REQUIRE(int_t(+1).is_pos());
//             REQUIRE(int_t(-1).is_neg());
//             REQUIRE_FALSE(int_t(+1).is_neg());

//             REQUIRE(int_t(-1).abs_checked() == 1);
//             REQUIRE(int_t(1).abs_checked() == 1);

//             REQUIRE(int_t(-1).is_abs_safe());
//             REQUIRE_FALSE(int_t::min().is_abs_safe());

//             REQUIRE_THROWS_AS(int_t::min().abs_checked(), contract_violation_exception);
//             REQUIRE_NOTHROW(int_t::min().abs_unchecked());

//             REQUIRE(int_t(-1).neg_checked() == 1);
//             REQUIRE(int_t(1).neg_checked() == -1);

//             REQUIRE(int_t(-1).is_neg_safe());
//             REQUIRE_FALSE(int_t::min().is_neg_safe());

//             REQUIRE_THROWS_AS(int_t::min().neg_checked(), contract_violation_exception);
//             REQUIRE_NOTHROW(int_t::min().neg_unchecked());

//             REQUIRE(int_t(int_min).sign() == -1);
//             REQUIRE(int_t(int_max).sign() == 1);
//         }
//     }
// }
