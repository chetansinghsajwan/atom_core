#pragma once
#include "MutRangeExtensions.h"
#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// # To Do: Implement this.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RDynamicRange = RMutRange<TRange>;

    template <typename T>
    class DynamicRangeExtensionsImpl
    {};

    template <typename T>
    class _DynamicRangeExtensionsImpl
    {};

    template <typename T>
    class DynamicRangeExtensions
    {
        using _TImpl = DynamicRangeExtensionsImpl<T>;

    public:
        using TElem = typename _TImpl::TElem;
        using TMutIter = typename _TImpl::TMutIter;
        using TMutIterEnd = typename _TImpl::TMutIterEnd;

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Removal
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

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
        _TImpl _impl;
    };
}
