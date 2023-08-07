#include "catch2/catch_all.hpp"
#include "Atom/Memory/ObjectBox.h"

using namespace Atom;

TEST_CASE("ObjectBox.Implementation")
{
    using TBox = ObjectBox<true,  true,  true,  50, DefaultMemAllocator>;

    TBox box0 = 10;
    TBox box1 = 10;

    CHECK(box0.GetObject<i32>() == 10);
    CHECK(box1.GetObject<i32>() == 10);

    box0 = 20;

    CHECK(box0.GetObject<i32>() == 20);
    CHECK(box1.GetObject<i32>() == 10);

    box0 = box1;

    CHECK(box0.GetObject<i32>() == 10);
}

TEST_CASE("ObjectBox.Conversion")
{
    using TBox1 = ObjectBox<true,  true,  true,  50, DefaultMemAllocator>;
    using TBox2 = ObjectBox<true,  true,  false, 50, DefaultMemAllocator>;
    using TBox3 = ObjectBox<true,  false, true,  50, DefaultMemAllocator>;
    using TBox4 = ObjectBox<true,  false, false, 50, DefaultMemAllocator>;
    using TBox5 = ObjectBox<false, true,  true,  50, DefaultMemAllocator>;
    using TBox6 = ObjectBox<false, true,  false, 50, DefaultMemAllocator>;
    using TBox7 = ObjectBox<false, false, true,  50, DefaultMemAllocator>;
    using TBox8 = ObjectBox<false, false, false, 50, DefaultMemAllocator>;

    TBox1 box1 = 0;
    TBox2 box2 = 0;
    TBox3 box3 = 0;
    TBox4 box4 = 0;
    TBox5 box5 = 0;
    TBox6 box6 = 0;
    TBox7 box7 = 0;
    TBox8 box8 = 0;

    ////////////////// | y | y | y |
                    // |-----------|
    box1 = box1;    // | y | y | y |
    box1 = box2;    // | y | y | n |
    box1 = box3;    // | y | n | y |
    box1 = box4;    // | y | n | n |
    // box1 = box5;    // | n | y | y |
    // box1 = box6;    // | n | y | n |
    // box1 = box7;    // | n | n | y |
    // box1 = box8;    // | n | n | n |

    ////////////////// | y | y | n |
                    // |-----------|
    // box2 = box1;    // | y | y | y |
    box2 = box2;    // | y | y | n |
    // box2 = box3;    // | y | n | y |
    // box2 = box4;    // | y | n | n |
    // box2 = box5;    // | n | y | y |
    // box2 = box6;    // | n | y | n |
    // box2 = box7;    // | n | n | y |
    // box2 = box8;    // | n | n | n |

    ////////////////// | y | n | y |
                    // |-----------|
    box3 = box1;    // | y | y | y |
    box3 = box2;    // | y | y | n |
    box3 = box3;    // | y | n | y |
    box3 = box4;    // | y | n | n |
    // box3 = box5;    // | n | y | y |
    // box3 = box6;    // | n | y | n |
    // box3 = box7;    // | n | n | y |
    // box3 = box8;    // | n | n | n |

    ////////////////// | y | n | n |
                    // |-----------|
    box4 = box1;    // | y | y | y |
    box4 = box2;    // | y | y | n |
    box4 = box3;    // | y | n | y |
    box4 = box4;    // | y | n | n |
    // box4 = box5;    // | n | y | y |
    // box4 = box6;    // | n | y | n |
    // box4 = box7;    // | n | n | y |
    // box4 = box8;    // | n | n | n |

    //////////////////////// | n | y | y |
                          // |-----------|
    box5 = mov(box1);    // | y | y | y |
    box5 = mov(box2);    // | y | y | n |
    box5 = mov(box3);    // | y | n | y |
    box5 = mov(box4);    // | y | n | n |
    box5 = mov(box5);    // | n | y | y |
    box5 = mov(box6);    // | n | y | n |
    box5 = mov(box7);    // | n | n | y |
    box5 = mov(box8);    // | n | n | n |

    ////////////////// | n | y | n |
                    // |-----------|
    // box6 = mov(box1);    // | y | y | y |
    box6 = mov(box2);    // | y | y | n |
    // box6 = mov(box3);    // | y | n | y |
    // box6 = mov(box4);    // | y | n | n |
    // box6 = mov(box5);    // | n | y | y |
    box6 = mov(box6);    // | n | y | n |
    // box6 = mov(box7);    // | n | n | y |
    // box6 = mov(box8);    // | n | n | n |
}