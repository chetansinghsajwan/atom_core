#pragma once
#include "atom/range.h"

namespace atom
{
    template <typename tout, typename type>
    concept routput = requires(tout out, const type& in_copy, type&& in_move, range_req_mock<type> in_range) {
        {
            out += in_copy
        };
        {
            out += mov(in_move)
        };
        {
            out += in_range
        };
    };
}
