#pragma once
#include "Atom/Range.h"
#include "Atom/Text/BasicChar.h"
#include "Atom/Text/BasicRune.h"

namespace Atom
{
    template <typename TEncoding_, typename TContainer>
    class _StrImpl: public TContainer
    {
    public:
        using TEncoding            = TEncoding_;

        using TChar                = BasicChar<TEncoding>;
        using TIter                = typename TContainer::TIter;
        using TIterEnd             = typename TContainer::TIterEnd;
        // using TMutIter             = typename TContainer::TMutIter;
        // using TMutIterEnd          = typename TContainer::TMutIterEnd;

        using TRune                = BasicRune<TEncoding>;
        // using TRuneIter            = typename TContainer::TRuneIter;
        // using TRuneIterEnd         = typename TContainer::TRuneIterEnd;
        // using TMutRuneIter         = typename TContainer::TMutRuneIter;
        // using TMutRuneIterEnd      = typename TContainer::TMutRuneIterEnd;

    public:
        using TContainer::TContainer;
        using TContainer::operator=;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr ctor _StrImpl(const TChar(&arr)[count]):
            TContainer() { }
    };
}