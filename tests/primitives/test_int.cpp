import atom.core;
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_template_test_macros.hpp"

using namespace atom;

TEMPLATE_TEST_CASE(
    "atom.core.int", "[primitives]", i8, i16, i32, i64, isize, imax, u8, u16, u32, u64, usize, umax)
{
    using int_type = TestType;

    SECTION("addition")
    {
        int_type n = 0;

        n = n + 1;
        REQUIRE(n == 1);

        n += 1;
        REQUIRE(n == 2);

        REQUIRE_THROWS_AS(n + int_type::max(), contract_violation_exception);
    }

    SECTION("subtraction")
    {
        int_type n = 0;

        n = n - 1;
        REQUIRE(n == -1);

        n -= 1;
        REQUIRE(n == -2);

        REQUIRE_THROWS_AS(n - int_type::min(), contract_violation_exception);
    }

    SECTION("multiplication")
    {
        int_type n = 1;

        n = n * 2;
        REQUIRE(n == 2);

        n *= 2;
        REQUIRE(n == 4);

        REQUIRE_THROWS_AS(int_type::max().div(2).add(1).mul(2), contract_violation_exception);
    }

    SECTION("division")
    {
        REQUIRE_THROWS_AS(int_type::min().div(-1), contract_violation_exception);
    }
}
