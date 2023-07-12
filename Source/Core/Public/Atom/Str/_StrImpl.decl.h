#pragma once
#include "Atom/Range.h"
#include "Atom/Text/BasicChar.h"
#include "Atom/Text/BasicRune.h"

namespace Atom
{
    template <tname _TEncoding, tname _TContainer>
    class _StrImpl: public _TContainer
    {
    public:
        using TEncoding            = _TEncoding;

        using TChar                = BasicChar<TEncoding>;
        using TIter                = tname _TContainer::TIter;
        using TIterEnd             = tname _TContainer::TIterEnd;
        // using TMutIter             = tname _TContainer::TMutIter;
        // using TMutIterEnd          = tname _TContainer::TMutIterEnd;

        using TRune                = BasicRune<TEncoding>;
        // using TRuneIter            = tname _TContainer::TRuneIter;
        // using TRuneIterEnd         = tname _TContainer::TRuneIterEnd;
        // using TMutRuneIter         = tname _TContainer::TMutRuneIter;
        // using TMutRuneIterEnd      = tname _TContainer::TMutRuneIterEnd;

    public:
        using _TContainer::_TContainer;
        using _TContainer::operator =;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr _StrImpl(const TChar(&arr)[count]): _TContainer(nullptr) { }
    };
}