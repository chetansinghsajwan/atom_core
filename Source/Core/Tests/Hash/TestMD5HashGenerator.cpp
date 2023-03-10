#include "catch2/catch_all.hpp"

#include "Atom/Hash/MD5HashGenerator.h"

using namespace Atom;

TEST_CASE("Atom/MD5HashGenerator")
{
    int data[] = { 0, 1, 2, 3, 4, 5 };

    MD5Hash out;
    MD5HashGenerator generator = MD5HashGenerator();

    generator.AddData(data, sizeof(int) * 6);
    generator.Generate(out);
}