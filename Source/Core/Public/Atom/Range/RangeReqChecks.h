#pragma once
#include "RangeReq.h"

//// -----------------------------------------------------------------------------------------------
#pragma region Implementations

#define _ATOM_SATISFIES_RANGE(RRange, TRange) \
    static_assert(RRange<TRange>, \
    "{" #TRange "} does not satisfy {" #RRange "} requirements.");

#define _ATOM_SATISFIES_RANGE_OF(RRange, TRange, T) \
    static_assert(RRange<TRange, T>, \
    "{" #TRange "} does not satisfy {" #RRange "} requirements for type {" #T "}.");

#define _ATOM_SATISFIES_RANGE_TEMP(RRange, TRange) \
    static_assert(RRange<TRange<int>, int>, \
    "{" #TRange "} does not satisfy {" #RRange "} requirements.");

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region Checks

#define ATOM_SATISFIES_RANGE(TRange)                    _ATOM_SATISFIES_RANGE(RRange, TRange)
#define ATOM_SATISFIES_RANGE_OF(TRange, T)              _ATOM_SATISFIES_RANGE_OF(RRangeOf, TRange, T)
#define ATOM_SATISFIES_RANGE_TEMP(TRange)               _ATOM_SATISFIES_RANGE_TEMP(RRangeOf, TRange)

#define ATOM_SATISFIES_MUT_RANGE(TRange)                _ATOM_SATISFIES_RANGE(RMutRange, TRange)
#define ATOM_SATISFIES_MUT_RANGE_OF(TRange, T)          _ATOM_SATISFIES_RANGE_OF(RMutRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_RANGE_TEMP(TRange)           _ATOM_SATISFIES_RANGE_TEMP(RMutRangeOf, TRange)

#define ATOM_SATISFIES_FWD_RANGE(TRange)                _ATOM_SATISFIES_RANGE(RFwdRange, TRange)
#define ATOM_SATISFIES_FWD_RANGE_OF(TRange, T)          _ATOM_SATISFIES_RANGE_OF(RFwdRangeOf, TRange, T)
#define ATOM_SATISFIES_FWD_RANGE_TEMP(TRange)           _ATOM_SATISFIES_RANGE_TEMP(RFwdRangeOf, TRange)

#define ATOM_SATISFIES_MUT_FWD_RANGE(TRange)            _ATOM_SATISFIES_RANGE(RMutFwdRange, TRange)
#define ATOM_SATISFIES_MUT_FWD_RANGE_OF(TRange, T)      _ATOM_SATISFIES_RANGE_OF(RMutFwdRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_FWD_RANGE_TEMP(TRange)       _ATOM_SATISFIES_RANGE_TEMP(RMutFwdRangeOf, TRange)

#define ATOM_SATISFIES_BIDI_RANGE(TRange)               _ATOM_SATISFIES_RANGE(RBidiRange, TRange)
#define ATOM_SATISFIES_BIDI_RANGE_OF(TRange, T)         _ATOM_SATISFIES_RANGE_OF(RBidiRangeOf, TRange, T)
#define ATOM_SATISFIES_BIDI_RANGE_TEMP(TRange)          _ATOM_SATISFIES_RANGE_TEMP(RBidiRangeOf, TRange)

#define ATOM_SATISFIES_MUT_BIDI_RANGE(TRange)           _ATOM_SATISFIES_RANGE(RMutBidiRange, TRange)
#define ATOM_SATISFIES_MUT_BIDI_RANGE_OF(TRange, T)     _ATOM_SATISFIES_RANGE_OF(RMutBidiRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_BIDI_RANGE_TEMP(TRange)      _ATOM_SATISFIES_RANGE_TEMP(RMutBidiRangeOf, TRange)

#define ATOM_SATISFIES_JUMP_RANGE(TRange)               _ATOM_SATISFIES_RANGE(RJumpRange, TRange)
#define ATOM_SATISFIES_JUMP_RANGE_OF(TRange, T)         _ATOM_SATISFIES_RANGE_OF(RJumpRangeOf, TRange, T)
#define ATOM_SATISFIES_JUMP_RANGE_TEMP(TRange)          _ATOM_SATISFIES_RANGE_TEMP(RJumpRangeOf, TRange)

#define ATOM_SATISFIES_MUT_JUMP_RANGE(TRange)           _ATOM_SATISFIES_RANGE(RMutJumpRange, TRange)
#define ATOM_SATISFIES_MUT_JUMP_RANGE_OF(TRange, T)     _ATOM_SATISFIES_RANGE_OF(RMutJumpRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_JUMP_RANGE_TEMP(TRange)      _ATOM_SATISFIES_RANGE_TEMP(RMutJumpRangeOf, TRange)

#define ATOM_SATISFIES_ARR_RANGE(TRange)                _ATOM_SATISFIES_RANGE(RArrRange, TRange)
#define ATOM_SATISFIES_ARR_RANGE_OF(TRange, T)          _ATOM_SATISFIES_RANGE_OF(RArrRangeOf, TRange, T)
#define ATOM_SATISFIES_ARR_RANGE_TEMP(TRange)           _ATOM_SATISFIES_RANGE_TEMP(RArrRangeOf, TRange)

#define ATOM_SATISFIES_MUT_ARR_RANGE(TRange)            _ATOM_SATISFIES_RANGE(RMutArrRange, TRange)
#define ATOM_SATISFIES_MUT_ARR_RANGE_OF(TRange, T)      _ATOM_SATISFIES_RANGE_OF(RMutArrRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_ARR_RANGE_TEMP(TRange)       _ATOM_SATISFIES_RANGE_TEMP(RMutArrRangeOf, TRange)

#define ATOM_SATISFIES_REV_RANGE(TRange)                _ATOM_SATISFIES_RANGE(RRevRange, TRange)
#define ATOM_SATISFIES_REV_RANGE_OF(TRange, T)          _ATOM_SATISFIES_RANGE_OF(RRevRangeOf, TRange, T)
#define ATOM_SATISFIES_REV_RANGE_TEMP(TRange)           _ATOM_SATISFIES_RANGE_TEMP(RRevRangeOf, TRange)

#define ATOM_SATISFIES_MUT_REV_RANGE(TRange)            _ATOM_SATISFIES_RANGE(RMutRevRange, TRange)
#define ATOM_SATISFIES_MUT_REV_RANGE_OF(TRange, T)      _ATOM_SATISFIES_RANGE_OF(RMutRevRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_REV_RANGE_TEMP(TRange)       _ATOM_SATISFIES_RANGE_TEMP(RMutRevRangeOf, TRange)

#define ATOM_SATISFIES_COMMON_RANGE(TRange)             _ATOM_SATISFIES_RANGE(RCommonRange, TRange)
#define ATOM_SATISFIES_COMMON_RANGE_OF(TRange, T)       _ATOM_SATISFIES_RANGE_OF(RCommonRangeOf, TRange, T)
#define ATOM_SATISFIES_COMMON_RANGE_TEMP(TRange)        _ATOM_SATISFIES_RANGE_TEMP(RCommonRangeOf, TRange)

#define ATOM_SATISFIES_MUT_COMMON_RANGE(TRange)         _ATOM_SATISFIES_RANGE(RMutCommonRange, TRange)
#define ATOM_SATISFIES_MUT_COMMON_RANGE_OF(TRange, T)   _ATOM_SATISFIES_RANGE_OF(RMutCommonRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_COMMON_RANGE_TEMP(TRange)    _ATOM_SATISFIES_RANGE_TEMP(RMutCommonRangeOf, TRange)

#pragma endregion
//// -----------------------------------------------------------------------------------------------