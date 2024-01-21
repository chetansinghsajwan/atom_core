import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom::hash::sha1_hash")
{
    SECTION("hash comparision")
    {
        sha1_hash hash1 = { 0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef,
            0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09 };

        sha1_hash hash2 = { 0xaa, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef,
            0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09 };

        REQUIRE(hash1.eq(hash1));
        REQUIRE(not hash1.eq(hash2));
    }

    SECTION("null hash")
    {
        sha1_hash hash;
        sha1_hash null_hash = sha1_hash::null;

        REQUIRE(null_hash.eq(sha1_hash::null));

        hash = sha1_hash::null;

        REQUIRE(hash.eq(sha1_hash::null));
        REQUIRE(hash.eq(null_hash));
    }
}

TEST_CASE("atom::hash::sha1_hash_parser")
{
    SECTION("string to hash")
    {
        sha1_hash hash =
            sha1_hash_parser().parse(make_range("da39a3ee5e6b4b0d3255bfef95601890afd80709"));

        sha1_hash expected = { 0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf,
            0xef, 0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09 };

        REQUIRE(hash.eq(expected));
    }

    SECTION("invalid hash")
    {
        //! won't compile
        // sha1_hash hash = sha1_hash_parser()
        //     .parse(make_range("da3"));
        //
        // REQUIRE(hash.eq(sha1_hash));
    }
}

TEST_CASE("atom::hash::sha1_hash_stringifier")
{
    sha1_hash hash = { 0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef, 0x95,
        0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09 };

    string str = sha1_hash_stringifier().to_string(hash);

    string_view expected = make_range("da39a3ee5e6b4b0d3255bfef95601890afd80709");

    REQUIRE(str.eq(expected));
}

TEST_CASE("atom::hash::sha1_hash_generator")
{
    // tests if the sha-1 hash of an empty input string is correct.
    SECTION("test_empty_input")
    {
        const char input[] = "";

        sha1_hash hash = sha1_hash_generator().generate();

        sha1_hash expected =
            sha1_hash_parser().parse(make_range("da39a3ee5e6b4b0d3255bfef95601890afd80709"));

        REQUIRE(hash.eq(expected));
    }

    // tests if the sha-1 hash of a single block input string is correct.
    SECTION("test_single_block_input")
    {
        const char input[] = "the quick brown fox jumps over the lazy dog";

        sha1_hash hash = sha1_hash_generator().process_bytes(input, sizeof(input) - 1).generate();

        sha1_hash expected =
            sha1_hash_parser().parse(make_range("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"));

        REQUIRE(hash.eq(expected));
    }

    // tests if the sha-1 hash of a multi-block input string is correct.
    SECTION("test_multi_block_input")
    {
        const char input[] =
            "lorem ipsum dolor sit amet, consectetur adipiscing elit, "
            "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
            "ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
            "nisi ut aliquip ex ea commodo consequat. duis aute irure dolor in "
            "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
            "excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia "
            "deserunt mollit anim id est laborum.";

        sha1_hash hash = sha1_hash_generator().process_bytes(input, sizeof(input) - 1).generate();

        sha1_hash expected =
            sha1_hash_parser().parse(make_range("cd36b370758a259b34845084a6cc38473cb95e27"));

        REQUIRE(hash.eq(expected));
    }
}
