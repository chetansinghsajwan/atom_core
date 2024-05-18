export module atom.core:core.byte;

namespace atom
{
    export using byte = unsigned char;

    export consteval auto byte_get_bit_count()
    {
        return 8;
    }
}
