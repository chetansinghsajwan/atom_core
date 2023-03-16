#include "catch2/catch_all.hpp"

#include "Atom/Hash/SHA1HashGenerator.h"

using namespace Atom;

TEST_CASE("Atom/SHA1HashGenerator")
{
    int data[] = { 0, 1, 2, 3, 4, 5 };

    SHA1Hash out;
    SHA1HashGenerator generator = SHA1HashGenerator();

    generator.ProcessBytes(data, sizeof(int) * 6);
    generator.Generate(out);
}