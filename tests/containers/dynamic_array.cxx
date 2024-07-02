module;
#include "catch2/catch_test_macros.hpp"

module atom_core.tests:dynamic_array;

import atom_core;
import :tracked_type;

using namespace atom;
using namespace atom::tests;

TEST_CASE("atom_core.dynamic_array")
{
//     using tracked_i32 = tracked_type_of<i32>;
//     using dynamic_array_tracked_i32 = dynamic_array<tracked_i32, default_mem_allocator>;

//     SECTION("default constructor")
//     {
//         dynamic_array_tracked_i32 arr;
//         REQUIRE(arr.get_data() == nullptr);
//         REQUIRE(arr.get_count() == 0);
//         REQUIRE(arr.get_capacity() == 0);
//     }

//     SECTION("copy constructor")
//     {
//         auto input = ranges::from({ tracked_i32(0), tracked_i32(1), tracked_i32(2) });
//         dynamic_array_tracked_i32 arr1 = input;
//         dynamic_array_tracked_i32 arr2(arr1);

//         REQUIRE(arr1.get_count() == 3);
//         REQUIRE(arr2.get_count() == arr1.get_count());
//         REQUIRE(arr2.get_at(0).value == 0);
//         REQUIRE(arr2.get_at(1).value == 1);
//         REQUIRE(arr2.get_at(2).value == 2);
//         REQUIRE(arr2.get_at(0).last_op == tracked_type::operation::copy_constructor);
//         REQUIRE(arr2.get_at(1).last_op == tracked_type::operation::copy_constructor);
//         REQUIRE(arr2.get_at(2).last_op == tracked_type::operation::copy_constructor);
//     }

//     SECTION("move constructor")
//     {
//         auto input = ranges::from({ tracked_i32(0), tracked_i32(1), tracked_i32(2) });
//         dynamic_array_tracked_i32 arr1 = input;

//         const tracked_i32* old_data = arr1.get_data();
//         usize old_count = arr1.get_count();
//         usize old_capacity = arr1.get_capacity();
//         arr1.get_at(0).last_op = tracked_type::operation::none;
//         arr1.get_at(1).last_op = tracked_type::operation::none;
//         arr1.get_at(2).last_op = tracked_type::operation::none;

//         dynamic_array_tracked_i32 arr2(move(arr1));

//         REQUIRE(arr1.get_data() == nullptr);
//         REQUIRE(arr1.get_count() == 0);
//         REQUIRE(arr1.get_capacity() == 0);

//         REQUIRE(arr2.get_data() == old_data);
//         REQUIRE(arr2.get_count() == old_count);
//         REQUIRE(arr2.get_capacity() == old_capacity);
//         REQUIRE(arr2.get_at(0).last_op == tracked_type::operation::none);
//         REQUIRE(arr2.get_at(1).last_op == tracked_type::operation::none);
//         REQUIRE(arr2.get_at(2).last_op == tracked_type::operation::none);
//     }

//     SECTION("range constructor")
//     {
//         SECTION("copy range")
//         {
//             auto input = ranges::from({ tracked_i32(0), tracked_i32(1), tracked_i32(2) });
//             dynamic_array_tracked_i32 arr = input;

//             REQUIRE(arr.get_count() == 3);
//             REQUIRE(arr.get_at(0).value == 0);
//             REQUIRE(arr.get_at(1).value == 1);
//             REQUIRE(arr.get_at(2).value == 2);
//             REQUIRE(arr.get_at(0).last_op == tracked_type::operation::copy_constructor);
//             REQUIRE(arr.get_at(1).last_op == tracked_type::operation::copy_constructor);
//             REQUIRE(arr.get_at(2).last_op == tracked_type::operation::copy_constructor);
//         }

//         SECTION("move range")
//         {
//             auto input = ranges::from({ tracked_i32(0), tracked_i32(1), tracked_i32(2) });
//             dynamic_array_tracked_i32 arr = move(input);

//             REQUIRE(arr.get_count() == 3);
//             REQUIRE(arr.get_at(0).value == 0);
//             REQUIRE(arr.get_at(1).value == 1);
//             REQUIRE(arr.get_at(2).value == 2);
//             REQUIRE(arr.get_at(0).last_op == tracked_type::operation::move_constructor);
//             REQUIRE(arr.get_at(1).last_op == tracked_type::operation::move_constructor);
//             REQUIRE(arr.get_at(2).last_op == tracked_type::operation::move_constructor);
//         }
//     }

//     SECTION("destructor") {}

//     SECTION("copy operator") {}

//     SECTION("move operator") {}

//     SECTION("range operator") {}
}
