#pragma once
#include "atom/hash/private/t1_hash.h"
#include "atom/string/string.h"
#include "atom/string/string_converter.h"

namespace atom
{
    template <typename t1_hash>
    class _t1_hash_stringifier
    {
    public:
        constexpr auto to_string(const t1_hash& hash) -> string
        {
            string str;
            write_string(hash, str);
            return str;
        };

        template <typename tout>
        constexpr auto write_string(const t1_hash& hash, tout&& out)
            requires(routput<tout, char>)
        {
            // for (byte b : hash.bytes)
            // {
            //     stack_string<2> chars = math::hex_to_char(b);
            //     out += chars[0];
            //     out += chars[1];
            // }
        }
    };

    template <typename t1_hash>
        requires(rdefault_constructible<_t1_hash_stringifier<t1_hash>>)
    class string_converter<t1_hash>
    {
    public:
        constexpr auto convert(const t1_hash& hash) -> string
        {
            return _t1_hash_stringifier<t1_hash>().to_string(hash);
        }

        constexpr auto convert(const t1_hash& hash, routput<char> auto&& out)
        {
            return _t1_hash_stringifier<t1_hash>().write_string(hash, out);
        }
    };
}
