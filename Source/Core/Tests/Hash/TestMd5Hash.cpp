#include "catch2/catch_all.hpp"

#include "Atom/Hash/Md5Hash.h"
#include "Atom/Hash/Md5HashGenerator.h"
#include "Atom/Hash/Md5HashParser.h"
#include "Atom/Hash/Md5HashStringifier.h"

using namespace Atom;

TEST_CASE("Atom::Hash::Md5Hash")
{
    SECTION("Hash Comparision")
    {
        Md5Hash hash1 = { 0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef,
            0x95, 0x60, 0x18, 0x90 };

        Md5Hash hash2 = { 0xaa, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef,
            0x95, 0x60, 0x18, 0x90 };

        CHECK(hash1 == hash1);
        CHECK(hash1 != hash2);
    }

    SECTION("Null Hash")
    {
        Md5Hash hash;
        Md5Hash nullHash = Md5Hash::Null;

        CHECK(nullHash == Md5Hash::Null);

        hash = Md5Hash::Null;

        CHECK(hash == Md5Hash::Null);
        CHECK(hash == nullHash);
    }
}

TEST_CASE("Atom::Hash::Md5HashParser")
{
    SECTION("Str to Hash")
    {
        Md5Hash hash = Md5HashParser().Parse("da39a3ee5e6b4b0d3255bfef95601890");

        Md5Hash expected = { 0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef,
            0x95, 0x60, 0x18, 0x90 };

        CHECK(hash == expected);
    }

    SECTION("Invalid Hash")
    {
        //! Won't Compile
        // Md5Hash hash = Md5HashParser()
        //     .Parse("da3");
        //
        // CHECK(hash == Md5Hash);
    }
}

TEST_CASE("Atom::Hash::Md5HashStringifier")
{
    Md5Hash hash = { 0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef, 0x95,
        0x60, 0x18, 0x90 };

    Str str = Md5HashStringifier().ToStr(hash);

    StrView expected = "da39a3ee5e6b4b0d3255bfef95601890";

    CHECK(str == expected);
}

TEST_CASE("Atom::Hash::Md5HashGenerator")
{
    // Tests if the SHA-1 hash of an empty input string is correct.
    SECTION("TestEmptyInput")
    {
        constexpr char input[] = "";

        Md5Hash hash = Md5HashGenerator().Generate();

        Md5Hash expected = Md5HashParser().Parse("d41d8cd98f00b204e9800998ecf8427e");

        CHECK(hash == expected);
    }

    // Tests if the SHA-1 hash of a single block input string is correct.
    SECTION("TestSingleBlockInput")
    {
        constexpr char input[] = "The quick brown fox jumps over the lazy dog";

        Md5Hash hash = Md5HashGenerator().ProcessBytes(input, sizeof(input) - 1).Generate();

        Md5Hash expected = Md5HashParser().Parse("9e107d9d372bb6826bd81d3542a419d6");

        CHECK(hash == expected);
    }

    // Tests if the SHA-1 hash of a multi-block input string is correct.
    SECTION("TestMultiBlockInput")
    {
        constexpr char input[] =
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
            "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
            "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in "
            "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
            "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia "
            "deserunt mollit anim id est laborum.";

        Md5Hash hash = Md5HashGenerator().ProcessBytes(input, sizeof(input) - 1).Generate();

        Md5Hash expected = Md5HashParser().Parse("db89bb5ceab87f9c0fcc2ab36c189c2c");

        CHECK(hash == expected);
    }
}
