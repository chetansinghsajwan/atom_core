#include "catch2/catch_all.hpp"

#include "Atom/Hash/Sha1Hash.h"

using namespace Atom;

TEST_CASE("Atom::Hash::Sha1Hash")
{
    SECTION("Hash Comparision")
    {
        Sha1Hash hash1 =
        {
            0xd, 0xa, 0x3, 0x9, 0xa, 0x3, 0xe, 0xe, 0x5, 0xe, 0x6, 0xb, 0x4,
            0xb, 0x0, 0xd, 0x3, 0x2, 0x5, 0x5, 0xb, 0xf, 0xe, 0xf, 0x9, 0x5,
            0x6, 0x0,0x1, 0x8, 0x9, 0x0, 0xa, 0xf, 0xd, 0x8, 0x0, 0x7, 0x0, 0x9
        };

        Sha1Hash hash2 =
        {
            0xa, 0xa, 0x3, 0x9, 0xa, 0x3, 0xe, 0xe, 0x5, 0xe, 0x6, 0xb, 0x4,
            0xb, 0x0, 0xd, 0x3, 0x2, 0x5, 0x5, 0xb, 0xf, 0xe, 0xf, 0x9, 0x5,
            0x6, 0x0,0x1, 0x8, 0x9, 0x0, 0xa, 0xf, 0xd, 0x8, 0x0, 0x7, 0x0, 0x9
        };

        CHECK(hash1 == hash1);
        CHECK(hash1 != hash2);
    }

    SECTION("Null Hash")
    {
        Sha1Hash hash;
        Sha1Hash nullHash = null;

        CHECK(hash == null);
        CHECK(hash == nullHash);

        hash = null;

        CHECK(hash == null);
    }
}

TEST_CASE("Atom::Hash::Sha1HashParser")
{
    SECTION("String to Hash")
    {
        Sha1Hash hash = Sha1HashParser()
            .Parse("da39a3ee5e6b4b0d3255bfef95601890afd80709");

        Sha1Hash expected =
        {
            0xd, 0xa, 0x3, 0x9, 0xa, 0x3, 0xe, 0xe, 0x5, 0xe, 0x6, 0xb, 0x4,
            0xb, 0x0, 0xd, 0x3, 0x2, 0x5, 0x5, 0xb, 0xf, 0xe, 0xf, 0x9, 0x5,
            0x6, 0x0,0x1, 0x8, 0x9, 0x0, 0xa, 0xf, 0xd, 0x8, 0x0, 0x7, 0x0, 0x9
        };

        CHECK(hash == expected);
    }

    SECTION("Invalid Hash")
    {
        Sha1Hash hash = Sha1HashParser()
            .Parse("da3");

        CHECK(hash == Sha1Hash);
    }
}

TEST_CASE("Atom::Hash::Sha1HashGenerator")
{
    // Tests if the SHA-1 hash of an empty input string is correct.
    SECTION("TestEmptyInput")
    {
        const char input[] = "";

        Sha1Hash hash = Sha1HashGenerator()
            .ProcessBytes(input, sizeof(input));
            .GenerateHash();

        Sha1Hash expected = Sha1HashParser()
            .Parse("da39a3ee5e6b4b0d3255bfef95601890afd80709");

        CHECK(hash == expected);
    }

    // Tests if the SHA-1 hash of a single block input string is correct.
    SECTION("TestSingleBlockInput")
    {
        const char input[] = "The quick brown fox jumps over the lazy dog";

        Sha1Hash hash = Sha1HashGenerator()
            .ProcessBytes(input, sizeof(input));
            .GenerateHash();

        Sha1Hash expected = Sha1HashParser()
            .Parse("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");

        CHECK(hash == expected);
    }

    // Tests if the SHA-1 hash of a multi-block input string is correct.
    SECTION("TestMultiBlockInput")
    {
        const char input[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
            "Sed eget mi at justo ultrices faucibus a sed nulla. Integer vitae lacus vehicula, "
            "fringilla elit in, venenatis ipsum. Nullam vel odio lobortis, venenatis ex eget, "
            "pellentesque nibh. In hac habitasse platea dictumst. Vestibulum ante ipsum primis "
            "in faucibus orci luctus et ultrices posuere cubilia curae; Ut non dolor a libero "
            "interdum tincidunt. Quisque sed pharetra nulla. Proin lacinia neque eu nisl congue, "
            "id facilisis sapien ultrices. Suspendisse potenti.";

        Sha1Hash hash = Sha1HashGenerator()
            .ProcessBytes(input, sizeof(input));
            .GenerateHash();

        Sha1Hash expected = Sha1HashParser()
            .Parse("ca84c2dfaeed174dd7aa2939b3729c7ee8d56eb2");

        CHECK(hash == expected);
    }
}