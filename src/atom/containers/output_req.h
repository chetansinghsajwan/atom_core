#pragma once
#include "atom/range.h"

namespace atom
{
    template <typename output_type, typename type>
    concept routput = requires(output_type out, const type& in_copy, type&& in_move, range_req_mock<type> in_range) {
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
