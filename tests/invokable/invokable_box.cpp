#include "atom.core.h"
#include "catch2/catch_test_macros.hpp"
#include "catch2/benchmark/catch_benchmark.hpp"
#include <functional>

using namespace atom;

TEST_CASE("atom.core.invokable_box")
{
    SECTION("not_move_assignable result")
    {
        class not_move_assignable
        {
        public:
            not_move_assignable() = default;

            not_move_assignable(const not_move_assignable& other) = default;
            not_move_assignable(not_move_assignable&& other) = default;

            auto operator=(const not_move_assignable& other) -> not_move_assignable& = delete;
            auto operator=(not_move_assignable&& other) -> not_move_assignable& = delete;
        };

        invokable_box<not_move_assignable(i32)> invokable = [](i32 value) {
            return not_move_assignable();
        };
    }

    SECTION("invocation")
    {
        invokable_box<bool(i32)> lambda_invokable = [](i32 value) { return value == 1; };

        REQUIRE(lambda_invokable(0) == false);
        REQUIRE(lambda_invokable(1) == true);

        class lambda final
        {
        public:
            lambda(i32* captured_value)
                : _captured_value(captured_value)
            {}

            lambda(const lambda& other)
            {
                _captured_value = other._captured_value;
            }

            lambda(lambda&& other)
            {
                this->_captured_value = other._captured_value;
                other._captured_value = nullptr;
            }

            ~lambda() {}

        public:
            auto operator()() -> i32
            {
                return *_captured_value;
            }

        private:
            i32* _captured_value;
        };

        i32 captured_value = 10;
        invokable_box<i32()> capture_lambda_invokable = lambda(&captured_value);

        REQUIRE(capture_lambda_invokable() != 0);
        REQUIRE(capture_lambda_invokable() == captured_value);
    }
}

TEST_CASE("atom.core.invokable_box", "[benchmarks]")
{
    //// |-----------------------------------------------------------------------------------------|
    //// | benchmark name                                 samples       iterations    estimated    |
    //// |                                                mean          low mean      high mean    |
    //// |                                                std dev       low std dev   high std dev |
    //// |-----------------------------------------------------------------------------------------|
    //// | atom::invokable_box construction                         100          7253     1.4506 ms |
    //// |                                                  2.44725 ns    2.44685 ns    2.44778 ns |
    //// |                                               0.00232811 ns 0.00185345 ns 0.00373542 ns |
    //// |                                                                                         |
    //// | std::function construction                              100         11910      1.191 ms |
    //// |                                                  1.46872 ns    1.46849 ns    1.46896 ns |
    //// |                                               0.00118305 ns 0.00109721 ns 0.00130371 ns |
    //// |                                                                                         |
    //// | atom::invokable_box                                      100         12116     1.2116 ms |
    //// |                                                  1.50155 ns    1.47038 ns    1.57987 ns |
    //// |                                                 0.221244 ns  0.0197346 ns   0.420887 ns |
    //// |                                                                                         |
    //// | std::function                                           100         14547     1.4547 ms |
    //// |                                                  1.25248 ns      1.225 ns    1.32104 ns |
    //// |                                                 0.193768 ns  0.0177648 ns   0.356329 ns |
    //// |-----------------------------------------------------------------------------------------|

    BENCHMARK("atom::invokable_box [construction]")
    {
        invokable_box<bool(i32)> invokable = [](i32 value) { return value == 1; };

        return invokable;
    };

    BENCHMARK("std::function [construction]")
    {
        std::function<bool(i32)> function = [](i32 value) { return value == 1; };

        return function;
    };

    invokable_box<bool(i32)> invokable = [](i32 value) { return value == 1; };

    std::function<bool(i32)> function = [](i32 value) { return value == 1; };

    BENCHMARK("atom::invokable_box [invocation]")
    {
        return invokable(0);
    };

    BENCHMARK("std::function [invocation]")
    {
        return function(0);
    };
}
