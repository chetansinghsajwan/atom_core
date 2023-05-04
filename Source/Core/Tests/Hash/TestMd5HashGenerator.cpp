#include "catch2/catch_all.hpp"

#include "Atom/Hash/Md5Hash.h"
#include "Atom/Hash/Md5HashGenerator.h"

using namespace Atom;

TEST_CASE("Atom/Md5HashGenerator")
{
    int data[] = { 0, 1, 2, 3, 4, 5 };

    Md5HashGenerator generator = Md5HashGenerator();

    generator.ProcessBytes(data, sizeof(int) * 6);
    Md5Hash hash = generator.Generate();
}