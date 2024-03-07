#pragma once
#include <cstdint>

namespace atom
{
    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;
    using ifast8 = std::int_fast8_t;
    using ifast16 = std::int_fast16_t;
    using ifast32 = std::int_fast32_t;
    using ifast64 = std::int_fast64_t;
    using imax = std::intmax_t;
    using isize = std::ptrdiff_t;

    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;
    using ufast8 = std::uint_fast8_t;
    using ufast16 = std::uint_fast16_t;
    using ufast32 = std::uint_fast32_t;
    using ufast64 = std::uint_fast64_t;
    using umax = std::uintmax_t;
    using usize = std::size_t;
}
