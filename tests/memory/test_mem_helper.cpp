import atom.core;
#include "catch2/catch_test_macros.hpp"
// #include <memory>

using namespace atom;

TEST_CASE("atom::memory::mem_helper")
{
    mem_helper helper;

    mut_mem_ptr<void> src = std::malloc(100);
    mut_mem_ptr<void> dest = std::malloc(100);

    // todo: write tests.
    helper.fwd_copy_to({ src, 5 }, { dest, 10 });

    std::free(src.unwrap());
    std::free(dest.unwrap());
}
