export module atom_core:core.int_wrapper;

import std;

namespace atom
{
    export using i8 = std::int8_t;
    export using i16 = std::int16_t;
    export using i32 = std::int32_t;
    export using i64 = std::int64_t;
    export using i8fast = std::int_fast8_t;
    export using i16fast = std::int_fast16_t;
    export using i32fast = std::int_fast32_t;
    export using i64fast = std::int_fast64_t;
    export using imax = std::intmax_t;
    export using isize = std::ptrdiff_t;

    export using u8 = std::uint8_t;
    export using u16 = std::uint16_t;
    export using u32 = std::uint32_t;
    export using u64 = std::uint64_t;
    export using u8fast = std::uint_fast8_t;
    export using u16fast = std::uint_fast16_t;
    export using u32fast = std::uint_fast32_t;
    export using u64fast = std::uint_fast64_t;
    export using umax = std::uintmax_t;
    export using usize = std::size_t;
}
