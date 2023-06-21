#pragma once
#include "Atom/Range.h"
#include "Atom/Text/BasicChar.h"
#include "Atom/Text/BasicRune.h"

namespace Atom
{
    template <typename _TEncoding, typename _TContainer>
    class _StrImpl: public _TContainer
    {
    public:
        using TEncoding            = _TEncoding;

        using TChar                = BasicChar<TEncoding>;
        using TIter                = typename _TContainer::TIter;
        using TIterEnd             = typename _TContainer::TIterEnd;
        // using TMutIter             = typename _TContainer::TMutIter;
        // using TMutIterEnd          = typename _TContainer::TMutIterEnd;

        using TRune                = BasicRune<TEncoding>;
        // using TRuneIter            = typename _TContainer::TRuneIter;
        // using TRuneIterEnd         = typename _TContainer::TRuneIterEnd;
        // using TMutRuneIter         = typename _TContainer::TMutRuneIter;
        // using TMutRuneIterEnd      = typename _TContainer::TMutRuneIterEnd;

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