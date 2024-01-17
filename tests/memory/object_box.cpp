import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("object_box.implementation")
{
    using tbox = object_box<true, true, true, 50, default_mem_allocator>;

    tbox box0 = 10;
    tbox box1 = 10;

    REQUIRE(box0.get_object<i32>() == 10);
    REQUIRE(box1.get_object<i32>() == 10);

    box0 = 20;

    REQUIRE(box0.get_object<i32>() == 20);
    REQUIRE(box1.get_object<i32>() == 10);

    box0 = box1;

    REQUIRE(box0.get_object<i32>() == 10);
}

TEST_CASE("object_box.conversion")
{
    using tbox1 = object_box<true, true, true, 50, default_mem_allocator>;
    using tbox2 = object_box<true, true, false, 50, default_mem_allocator>;
    using tbox3 = object_box<true, false, true, 50, default_mem_allocator>;
    using tbox4 = object_box<true, false, false, 50, default_mem_allocator>;
    using tbox5 = object_box<false, true, true, 50, default_mem_allocator>;
    using tbox6 = object_box<false, true, false, 50, default_mem_allocator>;
    using tbox7 = object_box<false, false, true, 50, default_mem_allocator>;
    using tbox8 = object_box<false, false, false, 50, default_mem_allocator>;

    tbox1 box1 = 0;
    tbox2 box2 = 0;
    tbox3 box3 = 0;
    tbox4 box4 = 0;
    tbox5 box5 = 0;
    tbox6 box6 = 0;
    tbox7 box7 = 0;
    tbox8 box8 = 0;

    // | y | y | y |
    box1 = box1; // | y | y | y |
    box1 = box2; // | y | y | n |
    box1 = box3; // | y | n | y |
    box1 = box4; // | y | n | n |
    // box1 = box5;     // | n | y | y |
    // box1 = box6;     // | n | y | n |
    // box1 = box7;     // | n | n | y |
    // box1 = box8;     // | n | n | n |

    // | y | y | n |
    // box2 = box1;     // | y | y | y |
    box2 = box2; // | y | y | n |
    // box2 = box3;     // | y | n | y |
    // box2 = box4;     // | y | n | n |
    // box2 = box5;     // | n | y | y |
    // box2 = box6;     // | n | y | n |
    // box2 = box7;     // | n | n | y |
    // box2 = box8;     // | n | n | n |

    // | y | n | y |
    box3 = box1; // | y | y | y |
    box3 = box2; // | y | y | n |
    box3 = box3; // | y | n | y |
    box3 = box4; // | y | n | n |
    // box3 = box5;     // | n | y | y |
    // box3 = box6;     // | n | y | n |
    // box3 = box7;     // | n | n | y |
    // box3 = box8;     // | n | n | n |

    // | y | n | n |
    box4 = box1; // | y | y | y |
    box4 = box2; // | y | y | n |
    box4 = box3; // | y | n | y |
    box4 = box4; // | y | n | n |
    // box4 = box5;     // | n | y | y |
    // box4 = box6;     // | n | y | n |
    // box4 = box7;     // | n | n | y |
    // box4 = box8;     // | n | n | n |

    // | n | y | y |
    box5 = mov(box1); // | y | y | y |
    box5 = mov(box2); // | y | y | n |
    box5 = mov(box3); // | y | n | y |
    box5 = mov(box4); // | y | n | n |
    box5 = mov(box5); // | n | y | y |
    box5 = mov(box6); // | n | y | n |
    box5 = mov(box7); // | n | n | y |
    box5 = mov(box8); // | n | n | n |

    // | n | y | n |
    // box6 = mov(box1);    // | y | y | y |
    box6 = mov(box2); // | y | y | n |
    // box6 = mov(box3);    // | y | n | y |
    // box6 = mov(box4);    // | y | n | n |
    // box6 = mov(box5);    // | n | y | y |
    box6 = mov(box6); // | n | y | n |
    // box6 = mov(box7);    // | n | n | y |
    // box6 = mov(box8);    // | n | n | n |
}
