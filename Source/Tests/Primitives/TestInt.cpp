import atom.core;
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_template_test_macros.hpp"

using namespace Atom;

TEMPLATE_TEST_CASE("Atom.Core.Int", "[primitives]", i8, i16, i32, i64, isize, imax, u8, u16, u32, u64, usize, umax)
{
    using TInt = TestType;

    SECTION("Addition")
    {
        TInt n = 0;

        n = n + 1;
        REQUIRE(n == 1);

        n += 1;
        REQUIRE(n == 2);

        REQUIRE_THROWS_AS(n + TInt::Max(), ContractViolation);
    }

    SECTION("Subtraction")
    {
        TInt n = 0;

        n = n - 1;
        REQUIRE(n == -1);

        n -= 1;
        REQUIRE(n == -2);

        REQUIRE_THROWS_AS(n - TInt::Min(), ContractViolation);
    }

    SECTION("Multiplication")
    {
        TInt n = 1;

        n = n * 2;
        REQUIRE(n == 2);

        n *= 2;
        REQUIRE(n == 4);

        REQUIRE_THROWS_AS(TInt::Max().div(2).add(1).mul(2), ContractViolation);
    }

    SECTION("Division")
    {
        REQUIRE_THROWS_AS(TInt::Min().div(-1), ContractViolation);
    }
}