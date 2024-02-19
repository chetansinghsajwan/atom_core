import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("invokable/event")
{
    event_source<i32> event_source;
    event_source_view<i32> event = event_source;
    event_key key = event += [](i32 id) {};
}
