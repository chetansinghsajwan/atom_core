#include "catch2/catch_all.hpp"
#include "Atom/Invokable/Event.h"

using namespace Atom;

TEST_CASE("Invokable/Event")
{
    EventSource<int> eventSource;
    Event<int> event = eventSource;

    EventKey key = event += [](int id) {};
}