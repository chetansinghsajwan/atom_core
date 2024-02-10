export module atom.core:core;

export import :core.core;
export import :core.bitset;
export import :core.byte;
export import :core.char_wrapper;
export import :core.int_wrapper;
export import :core.float_wrapper;
export import :requirements;

import :std;

export namespace atom
{
    using nullptr_type = std::nullptr_t;

    template <typename... arg_types>
    class type_holder
    {};
}
