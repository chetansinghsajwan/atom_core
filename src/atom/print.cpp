export module atom.core:print;
import :string_view;
import :std;

namespace atom
{
    export auto print(string_view str)
    {
        std::string_view std_str(str.to_std_char_ptr(), str.count().to_unwrapped());
        std::cout << std_str;
    }

    export auto println(string_view str)
    {
        print(str);
        std::cout << std::endl;
    }
}
