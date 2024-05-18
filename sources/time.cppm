export module atom.core:time;

import std;

namespace atom
{
    export using time_point = std::chrono::system_clock::time_point;

    namespace time
    {
        export inline auto now()
        {
            return std::chrono::system_clock::now();
        }
    }
}
