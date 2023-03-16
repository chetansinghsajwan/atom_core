#include "catch2/catch_all.hpp"

#include "Atom/Hash/Md5Hash.h"

using namespace Atom;

TEST_CASE("Atom/Md5HashGenerator")
{
    int data[] = { 0, 1, 2, 3, 4, 5 };

    Md5Hash out;
    Md5HashGenerator generator = Md5HashGenerator();

    generator.ProcessBytes(data, sizeof(int) * 6);
    generator.Generate(out);
}