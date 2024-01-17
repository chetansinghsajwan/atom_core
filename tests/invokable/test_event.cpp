import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace Atom;

TEST_CASE("Invokable/Event")
{
    EventSource<i32> eventSource;
    IEvent<i32>& event = eventSource;

    EventKey key = event += [](i32 id) {};
}
