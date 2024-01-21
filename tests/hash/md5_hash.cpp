import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom::hash::md5_hash")
{
    SECTION("hash comparision")
    {
        md5_hash hash1 = { 0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef,
            0x95, 0x60, 0x18, 0x90 };

        md5_hash hash2 = { 0xaa, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef,
            0x95, 0x60, 0x18, 0x90 };

        REQUIRE(hash1.eq(hash1));
        REQUIRE(not hash1.eq(hash2));
    }

    SECTION("null hash")
    {
        md5_hash hash;
        md5_hash null_hash = md5_hash::null;

        REQUIRE(null_hash.eq(md5_hash::null));

        hash = md5_hash::null;

        REQUIRE(hash.eq(md5_hash::null));
        REQUIRE(hash.eq(null_hash));
    }
}

TEST_CASE("atom::hash::md5_hash_parser")
{
    SECTION("string to hash")
    {
        md5_hash hash = md5_hash_parser().parse(make_range("da39a3ee5e6b4b0d3255bfef95601890"));

        md5_hash expected = { 0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef,
            0x95, 0x60, 0x18, 0x90 };

        REQUIRE(hash.eq(expected));
    }

    SECTION("invalid hash")
    {
        //! won't compile
        // md5_hash hash = md5_hash_parser()
        //     .parse("da3");
        //
        // REQUIRE(hash.eq(md5_hash));
    }
}

TEST_CASE("atom::hash::md5_hash_stringifier")
{
    md5_hash hash = { 0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef, 0x95,
        0x60, 0x18, 0x90 };

    string str = md5_hash_stringifier().to_string(hash);

    string_view expected = make_range("da39a3ee5e6b4b0d3255bfef95601890");

    REQUIRE(str.eq(expected));
}

TEST_CASE("atom::hash::md5_hash_generator")
{
    // tests if the sha-1 hash of an empty input string is correct.
    SECTION("test_empty_input")
    {
        constexpr char input[] = "";

        md5_hash hash = md5_hash_generator().generate();

        md5_hash expected = md5_hash_parser().parse(make_range("d41d8cd98f00b204e9800998ecf8427e"));

        REQUIRE(hash.eq(expected));
    }

    // tests if the sha-1 hash of a single block input string is correct.
    SECTION("test_single_block_input")
    {
        constexpr char input[] = "the quick brown fox jumps over the lazy dog";

        md5_hash hash = md5_hash_generator().process_bytes(input, sizeof(input) - 1).generate();

        md5_hash expected = md5_hash_parser().parse(make_range("9e107d9d372bb6826bd81d3542a419d6"));

        REQUIRE(hash.eq(expected));
    }

    // tests if the sha-1 hash of a multi-block input string is correct.
    SECTION("test_multi_block_input")
    {
        constexpr char input[] =
            "lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor "
            "incididunt ut labore et dolore magna aliqua. ut enim ad minim veniam, quis nostrud "
            "exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. duis aute irure "
            "dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
            "pariatur. excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia "
            "deserunt mollit anim id est laborum.";

        md5_hash hash = md5_hash_generator().process_bytes(input, sizeof(input) - 1).generate();

        md5_hash expected = md5_hash_parser().parse(make_range("db89bb5ceab87f9c0fcc2ab36c189c2c"));

        REQUIRE(hash.eq(expected));
    }
}
