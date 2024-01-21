export module atom.core:random;
import :std;

export namespace atom
{
    using mt19937 = std::mt19937;

    template <typename int_type>
    using uniform_int_distribution = std::uniform_int_distribution<int_type>;

    using random_device = std::random_device;
}
