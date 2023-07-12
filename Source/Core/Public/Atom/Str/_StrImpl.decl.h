#pragma once
#include "Atom/Range.h"
#include "Atom/Text/BasicChar.h"
#include "Atom/Text/BasicRune.h"

namespace Atom
{
    template <tname _TEncoding, tname TContainer>
    class _StrImpl: public TContainer
    {
    public:
        using TEncoding            = _TEncoding;

        using TChar                = BasicChar<TEncoding>;
        using TIter                = tname TContainer::TIter;
        using TIterEnd             = tname TContainer::TIterEnd;
        // using TMutIter             = tname TContainer::TMutIter;
        // using TMutIterEnd          = tname TContainer::TMutIterEnd;

        using TRune                = BasicRune<TEncoding>;
        // using TRuneIter            = tname TContainer::TRuneIter;
        // using TRuneIterEnd         = tname TContainer::TRuneIterEnd;
        // using TMutRuneIter         = tname TContainer::TMutRuneIter;
        // using TMutRuneIterEnd      = tname TContainer::TMutRuneIterEnd;

    public:
        using TContainer::TContainer;
        using TContainer::operator =;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        cexpr ctor _StrImpl(const TChar(&arr)[count]):
            TContainer(nullptr) { }
    };
}