#pragma once
#include "output_req.h"

namespace atom
{
    template <typename type>
    class output_req_mock
    {
    public:
        template <typename u>
        auto operator+=(u&& el)
            requires(rsame_as_unqualified<u, type>);

    public:
        template <typename range_type>
        auto operator+=(const range_type& range)
            requires(rrange_of<range_type, type>);
    };
}
