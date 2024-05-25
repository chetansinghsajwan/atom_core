import std;
import atom.core;

using namespace atom;

auto main(int argc, char** args) -> int
{
    contract_asserts(false, "this is my msg.");

    return 0;
}
