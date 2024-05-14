import atom.core;

#include "catch2/catch_all.hpp"

using namespace atom;

TEST_CASE("atom.core.result")
{
    result<int, error> res = 0;
    res.on_value([](int value) {});
    res.on_value([](int& value) {});
    move(res).on_value([](int&& value) {});
    res.on_value([](const int& value) {});
    move(res).on_value([](const int&& value) {});
    res.on_value([](volatile int& value) {});
    move(res).on_value([](volatile int&& value) {});
    res.on_value([](const volatile int& value) {});
    move(res).on_value([](const volatile int&& value) {});
}
