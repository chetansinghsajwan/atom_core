module;
#include "catch2/catch_test_macros.hpp"

module atom_core.tests:memory_utils;

import atom_core;

using namespace atom;

TEST_CASE("atom::memory::memory_utils")
{
    void* src = std::malloc(100);
    void* dest = std::malloc(100);

    memory_utils::fwd_copy_to(src, 5, dest, 10);

    std::free(src);
    std::free(dest);
}
