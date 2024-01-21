import atom.core;
#include "helpers/tracked_type.h"
#include "catch2/catch_test_macros.hpp"

using namespace atom;
using namespace atom::tests;

TEST_CASE("atom.core.shared_ptr")
{
    SECTION("default constructor")
    {
        shared_ptr<tracked_type> ptr;

        REQUIRE(ptr.is_eq(nullptr));
    }

    SECTION("null constructor")
    {
        shared_ptr<tracked_type> ptr;

        REQUIRE(ptr.is_eq(nullptr));
    }

    SECTION("value constructor")
    {
        tracked_type val;
        shared_ptr<tracked_type> ptr(&val);

        REQUIRE(ptr.is_eq(&val));
        REQUIRE(ptr.get_count() == 1);
    }

    SECTION("copy constructor")
    {
        tracked_type val;
        shared_ptr<tracked_type> ptr0(&val);
        shared_ptr<tracked_type> ptr1(ptr0);

        REQUIRE(ptr1.is_eq(&val));
        REQUIRE(ptr1.get_count() == 2);
    }

    SECTION("move constructor")
    {
        tracked_type val;
        shared_ptr<tracked_type> ptr0(&val);
        shared_ptr<tracked_type> ptr1(mov(ptr0));

        REQUIRE(ptr0.is_eq(nullptr));
        REQUIRE(ptr0.get_count() == 0);
        REQUIRE(ptr1.is_eq(&val));
        REQUIRE(ptr1.get_count() == 2);
    }

    SECTION("destructor")
    {
        tracked_type val;

        {
            shared_ptr<tracked_type> ptr0(&val);

            {
                shared_ptr<tracked_type> ptr1(ptr0);
            }

            REQUIRE(val.last_op == tracked_type::eoperation::default_constructor);
        }

        REQUIRE(val.last_op == tracked_type::eoperation::destructor);
    }

    SECTION("null operator")
    {
        tracked_type val;
        shared_ptr<tracked_type> ptr0(&val);
        shared_ptr<tracked_type> ptr1(ptr0);

        ptr0 = nullptr;

        REQUIRE(ptr0.is_eq(nullptr));
        REQUIRE(ptr0.get_count() == 0);
        REQUIRE(ptr1.get_count() == 1);
        REQUIRE(val.last_op == tracked_type::eoperation::default_constructor);

        REQUIRE(ptr1.is_eq(nullptr));
        REQUIRE(ptr1.get_count() == 0);
        REQUIRE(val.last_op == tracked_type::eoperation::destructor);
    }

    SECTION("copy operator")
    {
        tracked_type val0;
        tracked_type val1;
        shared_ptr<tracked_type> ptr0(&val0);
        shared_ptr<tracked_type> ptr1(&val1);

        ptr1 = ptr0;

        REQUIRE(val1.last_op == tracked_type::eoperation::destructor);
        REQUIRE(ptr1.is_eq(&val0));
        REQUIRE(ptr1.get_count() == 2);
    }

    SECTION("move operator")
    {
        tracked_type val0;
        tracked_type val1;
        shared_ptr<tracked_type> ptr0(&val0);
        shared_ptr<tracked_type> ptr1(&val1);

        ptr1 = mov(ptr0);

        REQUIRE(val1.last_op == tracked_type::eoperation::destructor);
        REQUIRE(ptr0.is_eq(nullptr));
        REQUIRE(ptr0.get_count() == 0);
        REQUIRE(ptr1.is_eq(&val0));
        REQUIRE(ptr1.get_count() == 1);
    }

    SECTION("release()")
    {
        // same as null operator.
    }
}
