#pragma once
#include "atom/core/_std.h"
#include "atom/core/string/string_view.h"

namespace atom
{
    inline auto print(string_view str)
    {
        std::string_view std_str(str.get_data(), str.get_count().to_unwrapped());
        std::cout << std_str;
    }

    inline auto println(string_view str)
    {
        print(str);
        std::cout << std::endl;
    }
}
