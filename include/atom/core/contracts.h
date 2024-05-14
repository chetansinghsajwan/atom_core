#pragma once

// #include "libassert/assert.hpp"

// #define ATOM_EXPECTS(...) ASSUME(__VA_ARGS__)
// #define ATOM_ASSERTS(...) ASSERT(__VA_ARGS__)
// #define ATOM_ENSURES(...) ASSERT(__VA_ARGS__)
// #define ATOM_DEBUG_EXPECTS(...) ASSUME(__VA_ARGS__)
// #define ATOM_DEBUG_ASSERTS(...) DEBUG_ASSERT(__VA_ARGS__)
// #define ATOM_DEBUG_ENSURES(...) DEBUG_ASSERT(__VA_ARGS__)
// #define static_assert(...) static_assert(__VA_ARGS__)
// #define ATOM_DEBUG_PANIC(...) PANIC(__VA_ARGS__)
// #define ATOM_PANIC(...) PANIC(__VA_ARGS__)
// #define ATOM_UNREACHABLE(...) UNREACHABLE(__VA_ARGS__)

constexpr auto ATOM_EXPECTS(auto&&... args) {}

constexpr auto ATOM_ASSERTS(auto&&... args) {}

constexpr auto ATOM_ENSURES(auto&&... args) {}

constexpr auto ATOM_DEBUG_EXPECTS(auto&&... args) {}

constexpr auto ATOM_DEBUG_ASSERTS(auto&&... args) {}

constexpr auto ATOM_DEBUG_ENSURES(auto&&... args) {}

constexpr auto ATOM_DEBUG_PANIC(auto&&... args) {}

constexpr auto ATOM_PANIC(auto&&... args) {}

constexpr auto ATOM_UNREACHABLE(auto&&... args) {}
