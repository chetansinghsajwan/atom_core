#pragma once
#include "mut_range_extensions.h"
#include "atom/invokable/invokable.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// # to do: implement this.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rdynamic_range = rmut_range<range_type>;

    template <typename type>
    class dynamic_range_extensions_impl
    {};

    template <typename type>
    class _dynamic_range_extensions_impl
    {};

    template <typename type>
    class dynamic_range_extensions
    {
        using _timpl = dynamic_range_extensions_impl<type>;

    public:
        using elem_type = typename _timpl::elem_type;
        using mut_iter_type = typename _timpl::mut_iter_type;
        using mut_iter_end_type = typename _timpl::mut_iter_end_type;

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// removal
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tpred>
        constexpr auto remove_if(tpred&& pred) -> usize
            requires(rinvokable<tpred, bool(const elem_type&)>)
        {
            usize count = 0;
            auto it_end = _impl.iter_end();
            for (auto it = _impl.iter(); it != it_end; it.next())
            {
                if (pred(it.value()))
                {
                    it = _impl.remove_at(it);
                    count++;
                }
            }

            return count;
        }

    private:
        _timpl _impl;
    };
}
