export module atom.core:core.byte;

namespace atom
{
    export using _byte = unsigned char;
    export using byte = _byte;

    export consteval auto byte_get_bit_count()
    {
        return 8;
    }
}
