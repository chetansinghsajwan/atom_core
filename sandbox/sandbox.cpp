import atom.core;

using namespace atom;

auto print_cmd_args(i32 argc, char** args)
{
    for (i32 i = 1; i < argc; i++)
    {
        // std::cout << args[i.unwrap()] << std::endl;
    }
}

auto main(int argc, char** args) -> int
{
    print_cmd_args(argc, args);

    mem_ptr<void> ptr = nullptr;

    return 0;
}
