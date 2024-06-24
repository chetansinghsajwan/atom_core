export module atom.core;

export import std;

export import :types;
export import :contracts;
export import :core;
export import :core.enums.final;
export import :ranges;
export import :containers;
export import :strings;
export import :time;
export import :hash;
export import :filesystem;
export import :io;

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
export import :dynamic_buffer;

export
{
    using std::int16_t;
    using std::int32_t;
    using std::int64_t;
    using std::int8_t;
    using std::int_fast16_t;
    using std::int_fast32_t;
    using std::int_fast64_t;
    using std::int_fast8_t;
    using std::intmax_t;
    using std::ptrdiff_t;

    using std::size_t;
    using std::uint16_t;
    using std::uint32_t;
    using std::uint64_t;
    using std::uint8_t;
    using std::uint_fast16_t;
    using std::uint_fast32_t;
    using std::uint_fast64_t;
    using std::uint_fast8_t;
    using std::uintmax_t;
}
