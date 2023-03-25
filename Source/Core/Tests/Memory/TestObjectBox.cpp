#include "catch2/catch_all.hpp"
#include "Atom/Memory/ObjectBox.h"

using namespace Atom;

TEST_CASE("ObjectBox")
{
    ObjectBox<> box0 = 10;
    ObjectBox<> box1 = box0;

    CHECK(box0.GetObject<int>() == 10);
    CHECK(box1.GetObject<int>() == 10);

    box0 = 20;

    CHECK(box0.GetObject<int>() == 20);
    CHECK(box1.GetObject<int>() == 10);

    box0 = box1;

    CHECK(box0.GetObject<int>() == 10);
}