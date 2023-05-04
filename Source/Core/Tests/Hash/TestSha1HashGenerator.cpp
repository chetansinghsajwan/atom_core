#include "catch2/catch_all.hpp"

#include "Atom/Hash/Sha1Hash.h"
#include "Atom/Hash/Sha1HashGenerator.h"
#include "Atom/Hash/Sha1HashParser.h"
#include "Atom/Hash/Sha1HashStringifier.h"

using namespace Atom;

TEST_CASE("Atom::Hash::Sha1Hash")
{
    SECTION("Hash Comparision")
    {
        Sha1Hash hash1 =
        {
            0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55,
            0xbf, 0xef, 0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09
        };

        Sha1Hash hash2 =
        {
            0xaa, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55,
            0xbf, 0xef, 0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09
        };

        CHECK(hash1 == hash1);
        CHECK(hash1 != hash2);
    }

    SECTION("Null Hash")
    {
        Sha1Hash hash;
        Sha1Hash nullHash = Sha1Hash::Null;

        CHECK(hash == Sha1Hash::Null);
        CHECK(hash == nullHash);

        hash = Sha1Hash::Null;

        CHECK(hash == Sha1Hash::Null);
    }
}

TEST_CASE("Atom::Hash::Sha1HashParser")
{
    SECTION("String to Hash")
    {
        Sha1Hash hash = Sha1HashParser()
            .Parse(TEXT("da39a3ee5e6b4b0d3255bfef95601890afd80709"));

        Sha1Hash expected =
        {
            0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55,
            0xbf, 0xef, 0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09
        };

        CHECK(hash == expected);
    }

    SECTION("Invalid Hash")
    {
        //! Won't Compile
        // Sha1Hash hash = Sha1HashParser()
        //     .Parse(TEXT("da3"));
        // 
        // CHECK(hash == Sha1Hash);
    }
}

TEST_CASE("Atom::Hash::Sha1HashGenerator")
{
    // Tests if the SHA-1 hash of an empty input string is correct.
    SECTION("TestEmptyInput")
    {
        const char input[] = "";

        Sha1Hash hash = Sha1HashGenerator()
            .ProcessBytes(input, sizeof(input))
            .Generate();

        Sha1Hash expected = Sha1HashParser()
            .Parse(TEXT("da39a3ee5e6b4b0d3255bfef95601890afd80709"));

        CHECK(hash == expected);
    }

    // Tests if the SHA-1 hash of a single block input string is correct.
    SECTION("TestSingleBlockInput")
    {
        const char input[] = "The quick brown fox jumps over the lazy dog";

        Sha1Hash hash = Sha1HashGenerator()
            .ProcessBytes(input, sizeof(input))
            .Generate();

        Sha1Hash expected = Sha1HashParser()
            .Parse(TEXT("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"));

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
            .ProcessBytes(input, sizeof(input))
            .Generate();

        Sha1Hash expected = Sha1HashParser()
            .Parse(TEXT("ca84c2dfaeed174dd7aa2939b3729c7ee8d56eb2"));

        CHECK(hash == expected);
    }
}