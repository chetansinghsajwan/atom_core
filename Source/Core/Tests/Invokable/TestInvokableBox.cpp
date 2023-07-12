#include <functional>
#include "catch2/catch_all.hpp"
#include "Atom/Invokable.h"

using namespace Atom;

TEST_CASE("Atom/Invokable/InvokableBox")
{
    SECTION("NotMoveAssignable Result")
    {
        struct NotMoveAssignable
        {
            NotMoveAssignable() = default;

            NotMoveAssignable(const NotMoveAssignable& other) = default;
            NotMoveAssignable(NotMoveAssignable&& other) = default;

            NotMoveAssignable& operator = (const NotMoveAssignable& other) = delete;
            NotMoveAssignable& operator = (NotMoveAssignable&& other) = delete;
        };

        InvokableBox<NotMoveAssignable(int)> invokable = [](int value)
        {
            return NotMoveAssignable();
        };
    }

    SECTION("Invocation")
    {
        InvokableBox<bool(int)> lambdaInvokable = [](int value)
        {
            return value == 1;
        };

        CHECK(lambdaInvokable(0) == false);
        CHECK(lambdaInvokable(1) == true);

        class Lambda final
        {
        public:
            Lambda(int* capturedValue) noex:
                _capturedValue(capturedValue) { }

            Lambda(const Lambda& other) noex
            {
                _capturedValue = other._capturedValue;
            }

            Lambda(Lambda&& other) noex
            {
                this->_capturedValue = other._capturedValue;
                other._capturedValue = nullptr;
            }

            ~Lambda() { }

        public:
            int operator () ()
            {
                return *_capturedValue;
            }

        private:
            int* _capturedValue;
        };

        int capturedValue = 10;
        InvokableBox<int()> captureLambdaInvokable = Lambda(&capturedValue);

        CHECK(captureLambdaInvokable() != 0);
        CHECK(captureLambdaInvokable() == capturedValue);
    }
}

TEST_CASE("Atom/Invokable/InvokableBox", "[Benchmarks]")
{
    //// |-----------------------------------------------------------------------------------------|
    //// | BENCHMARK NAME                                 SAMPLES       ITERATIONS    ESTIMATED    |
    //// |                                                MEAN          LOW MEAN      HIGH MEAN    |
    //// |                                                STD DEV       LOW STD DEV   HIGH STD DEV |
    //// |-----------------------------------------------------------------------------------------|
    //// | Atom::InvokableBox Construction                         100          7253     1.4506 ms |
    //// |                                                  2.44725 ns    2.44685 ns    2.44778 ns |
    //// |                                               0.00232811 ns 0.00185345 ns 0.00373542 ns |
    //// |                                                                                         |
    //// | std::function Construction                              100         11910      1.191 ms |
    //// |                                                  1.46872 ns    1.46849 ns    1.46896 ns |
    //// |                                               0.00118305 ns 0.00109721 ns 0.00130371 ns |
    //// |                                                                                         |
    //// | Atom::InvokableBox                                      100         12116     1.2116 ms |
    //// |                                                  1.50155 ns    1.47038 ns    1.57987 ns |
    //// |                                                 0.221244 ns  0.0197346 ns   0.420887 ns |
    //// |                                                                                         |
    //// | std::function                                           100         14547     1.4547 ms |
    //// |                                                  1.25248 ns      1.225 ns    1.32104 ns |
    //// |                                                 0.193768 ns  0.0177648 ns   0.356329 ns |
    //// |-----------------------------------------------------------------------------------------|

    BENCHMARK("Atom::InvokableBox [Construction]")
    {
        InvokableBox<bool(int)> invokable = [](int value)
        {
            return value == 1;
        };

        return invokable;
    };

    BENCHMARK("std::function [Construction]")
    {
        std::function<bool(int)> function = [](int value)
        {
            return value == 1;
        };

        return function;
    };

    InvokableBox<bool(int)> invokable = [](int value)
    {
        return value == 1;
    };

    std::function<bool(int)> function = [](int value)
    {
        return value == 1;
    };

    BENCHMARK("Atom::InvokableBox [Invocation]")
    {
        return invokable(0);
    };

    BENCHMARK("std::function [Invocation]")
    {
        return function(0);
    };
}