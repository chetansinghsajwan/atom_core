import atom.core;

using namespace atom;

auto print(string_view str)
{
    std::string_view std_str(str.data().as_unsafe<char>().unwrap(), str.count().unwrap());
    std::cout << std_str;
}

auto println(string_view str)
{
    print(str);
    std::cout << std::endl;
}

auto main(int argc, char** args) -> int
{
    dynamic_array<i32> arr = make_range({ 0, 1, 2 });

    mem_ptr<i32> it = arr.data();
    mem_ptr<i32> end = it + arr.count();
    for (; it != end; it++)
    {
        std::cout << it->to_unwrapped() << std::endl;
    }

    return 0;
}
