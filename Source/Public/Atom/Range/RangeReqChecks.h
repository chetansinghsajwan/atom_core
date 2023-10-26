#pragma once
#include "RangeReq.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// Implementations
////
////////////////////////////////////////////////////////////////////////////////////////////////////

#define _ATOM_SATISFIES_RANGE(RRange, TRange)                                                      \
    static_assert(RRange<TRange>, "{" #TRange "} does not satisfy {" #RRange "} requirements.");

#define _ATOM_SATISFIES_RANGE_OF(RRange, TRange, T)                                                \
    static_assert(RRange<TRange, T>,                                                               \
        "{" #TRange "} does not satisfy {" #RRange "} requirements for type {" #T "}.");

#define _ATOM_SATISFIES_RANGE_TEMP(RRange, TRange)                                                 \
    static_assert(                                                                                 \
        RRange<TRange<i32>>, "{" #TRange "} does not satisfy {" #RRange "} requirements.");

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// Checks
////
////////////////////////////////////////////////////////////////////////////////////////////////////

#define ATOM_SATISFIES_RANGE(TRange) _ATOM_SATISFIES_RANGE(RRange, TRange)
#define ATOM_SATISFIES_RANGE_OF(TRange, T) _ATOM_SATISFIES_RANGE_OF(RRangeOf, TRange, T)
#define ATOM_SATISFIES_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RRange, TRange)

#define ATOM_SATISFIES_MUT_RANGE(TRange) _ATOM_SATISFIES_RANGE(RMutRange, TRange)
#define ATOM_SATISFIES_MUT_RANGE_OF(TRange, T) _ATOM_SATISFIES_RANGE_OF(RMutRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RMutRange, TRange)

#define ATOM_SATISFIES_FWD_RANGE(TRange) _ATOM_SATISFIES_RANGE(RFwdRange, TRange)
#define ATOM_SATISFIES_FWD_RANGE_OF(TRange, T) _ATOM_SATISFIES_RANGE_OF(RFwdRangeOf, TRange, T)
#define ATOM_SATISFIES_FWD_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RFwdRange, TRange)

#define ATOM_SATISFIES_MUT_FWD_RANGE(TRange) _ATOM_SATISFIES_RANGE(RMutFwdRange, TRange)
#define ATOM_SATISFIES_MUT_FWD_RANGE_OF(TRange, T)                                                 \
    _ATOM_SATISFIES_RANGE_OF(RMutFwdRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_FWD_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RMutFwdRange, TRange)

#define ATOM_SATISFIES_BIDI_RANGE(TRange) _ATOM_SATISFIES_RANGE(RBidiRange, TRange)
#define ATOM_SATISFIES_BIDI_RANGE_OF(TRange, T) _ATOM_SATISFIES_RANGE_OF(RBidiRangeOf, TRange, T)
#define ATOM_SATISFIES_BIDI_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RBidiRange, TRange)

#define ATOM_SATISFIES_MUT_BIDI_RANGE(TRange) _ATOM_SATISFIES_RANGE(RMutBidiRange, TRange)
#define ATOM_SATISFIES_MUT_BIDI_RANGE_OF(TRange, T)                                                \
    _ATOM_SATISFIES_RANGE_OF(RMutBidiRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_BIDI_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RMutBidiRange, TRange)

#define ATOM_SATISFIES_JUMP_RANGE(TRange) _ATOM_SATISFIES_RANGE(RJumpRange, TRange)
#define ATOM_SATISFIES_JUMP_RANGE_OF(TRange, T) _ATOM_SATISFIES_RANGE_OF(RJumpRangeOf, TRange, T)
#define ATOM_SATISFIES_JUMP_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RJumpRange, TRange)

#define ATOM_SATISFIES_MUT_JUMP_RANGE(TRange) _ATOM_SATISFIES_RANGE(RMutJumpRange, TRange)
#define ATOM_SATISFIES_MUT_JUMP_RANGE_OF(TRange, T)                                                \
    _ATOM_SATISFIES_RANGE_OF(RMutJumpRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_JUMP_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RMutJumpRange, TRange)

#define ATOM_SATISFIES_ARR_RANGE(TRange) _ATOM_SATISFIES_RANGE(RArrayRange, TRange)
#define ATOM_SATISFIES_ARR_RANGE_OF(TRange, T) _ATOM_SATISFIES_RANGE_OF(RArrayRangeOf, TRange, T)
#define ATOM_SATISFIES_ARR_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RArrayRange, TRange)

#define ATOM_SATISFIES_MUT_ARR_RANGE(TRange) _ATOM_SATISFIES_RANGE(RMutArrayRange, TRange)
#define ATOM_SATISFIES_MUT_ARR_RANGE_OF(TRange, T)                                                 \
    _ATOM_SATISFIES_RANGE_OF(RMutArrayRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_ARR_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RMutArrayRange, TRange)

#define ATOM_SATISFIES_REV_RANGE(TRange) _ATOM_SATISFIES_RANGE(RRevRange, TRange)
#define ATOM_SATISFIES_REV_RANGE_OF(TRange, T) _ATOM_SATISFIES_RANGE_OF(RRevRangeOf, TRange, T)
#define ATOM_SATISFIES_REV_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RRevRange, TRange)

#define ATOM_SATISFIES_MUT_REV_RANGE(TRange) _ATOM_SATISFIES_RANGE(RMutRevRange, TRange)
#define ATOM_SATISFIES_MUT_REV_RANGE_OF(TRange, T)                                                 \
    _ATOM_SATISFIES_RANGE_OF(RMutRevRangeOf, TRange, T)
#define ATOM_SATISFIES_MUT_REV_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RMutRevRange, TRange)

#define ATOM_SATISFIES_COMMON_RANGE(TRange) _ATOM_SATISFIES_RANGE(RCommonRange, TRange)
#define ATOM_SATISFIES_COMMON_RANGE_OF(TRange, T)                                                  \
    _ATOM_SATISFIES_RANGE_OF(RCommonRangeOf, TRange, T)
#define ATOM_SATISFIES_COMMON_RANGE_TEMP(TRange) _ATOM_SATISFIES_RANGE_TEMP(RCommonRange, TRange)

#define ATOM_SATISFIES_MUT_COMMON_RANGE(TRange) _ATOM_SATISFIES_RANGE(RMutCommonRange, TRange)
#define ATOM_SATISFIES_MUT_COMMON_RANGE_TEMP(TRange)                                               \
    _ATOM_SATISFIES_RANGE_TEMP(RMutCommonRange, TRange)
