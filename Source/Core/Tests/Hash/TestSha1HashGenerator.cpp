#include "catch2/catch_all.hpp"

#include "Atom/Hash/Sha1Hash.h"

using namespace Atom;

TEST_CASE("Atom/Sha1HashGenerator")
{
    int data[] = { 0, 1, 2, 3, 4, 5 };

    Sha1HashGenerator generator = Sha1HashGenerator();

    generator.ProcessBytes(data, sizeof(int) * 6);
    Sha1Hash hash = generator.Generate();
}