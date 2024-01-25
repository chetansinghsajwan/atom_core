export module atom.core:atomic;
import :std;

namespace atom
{
    export template <typename type>
    using atomic = std::atomic<type>;
}
