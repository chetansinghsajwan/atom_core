#include "Atom/Invokable/Event.h"
#include "catch2/catch_all.hpp"

using namespace Atom;

TEST_CASE("Invokable/Event")
{
    EventSource<i32> eventSource;
    IEvent<i32>& event = eventSource;

    EventKey key = event += [](i32 id) {};
}
