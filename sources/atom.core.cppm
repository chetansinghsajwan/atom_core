export module atom.core;

export import std;
import fmt;
import magic_enum;

export import :types;
export import :contracts;
export import :core;
export import :ranges;
export import :containers;
export import :strings;
export import :print;
export import :time;
export import :hash;
export import :math;
export import :filesystem;

export import :mem_helper;
export import :obj_helper;
export import :lock_guard;
export import :lockable;
export import :box;
export import :atomic;
export import :mutex;
export import :null_lockable;
export import :shared_ptr;
export import :unique_ptr;
export import :default_mem_allocator;
export import :legacy_mem_allocator;
export import :function_box;

export
{
    using std::int8_t;
    using std::int16_t;
    using std::int32_t;
    using std::int64_t;
    using std::int_fast8_t;
    using std::int_fast16_t;
    using std::int_fast32_t;
    using std::int_fast64_t;
    using std::intmax_t;
    using std::ptrdiff_t;

    using std::uint8_t;
    using std::uint16_t;
    using std::uint32_t;
    using std::uint64_t;
    using std::uint_fast8_t;
    using std::uint_fast16_t;
    using std::uint_fast32_t;
    using std::uint_fast64_t;
    using std::uintmax_t;
    using std::size_t;
}
