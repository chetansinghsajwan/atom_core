#pragma once
// #include "atom/invokable/invokable.h"
#include "range_req.h"

namespace atom
{
    class range_modifier
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename range_type, typename pred_type>
            requires rrange<range_type> and rinvokable<pred_type, bool(const typename range_type::elem_type&)>
        constexpr auto remove_if(range_type& range, pred_type&& pred) -> usize
        {
            usize count = 0;
            auto it_end = range.iter_end();
            for (auto it = range.iter(); it.eq(it_end); it.next())
            {
                if (pred(it.value()))
                {
                    it = range.remove_at(it);
                    count++;
                }
            }

            return count;
        }
    };
}
