#pragma once
#include "MutRangeTrait.h"
#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// # To Do: Implement this.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RDynamicRange = RMutRange<TRange>;

    template <typename T>
    class DynamicRangeTraitImpl
    {};

    template <typename T>
    class _DynamicRangeTraitImpl
    {};

    template <typename T>
    class DynamicRangeTrait
    {
        using _Impl = DynamicRangeTraitImpl<T>;

    public:
        using TElem = typename _Impl::TElem;
        using TMutIter = typename _Impl::TMutIter;
        using TMutIterEnd = typename _Impl::TMutIterEnd;

        ////
        ///-------------------------------------------------------------------------------------------
        //// Removal
        ////
        ///-------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TPred>
        constexpr auto removeIf(TPred&& pred) -> usize
            requires(RInvokable<TPred, bool(const TElem&)>)
        {
            usize count = 0;
            auto itEnd = _impl.iterEnd();
            for (auto it = _impl.iter(); it != itEnd; it.next())
            {
                if (pred(it.value()))
                {
                    it = _impl.removeAt(it);
                    count++;
                }
            }

            return count;
        }

    private:
        _Impl _impl;
    };
}
