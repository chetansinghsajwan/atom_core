#pragma once
#include "IterReq.h"

//// -----------------------------------------------------------------------------------------------
#pragma region Iter Assertion Implementation

#define _ATOM_SATISFIES_ITER(RIter, TIter)                                                         \
    static_assert(RIter<TIter>, "{" #TIter "} does not satisfy {" #RIter "} requirement.")

#define _ATOM_SATISFIES_ITER_OF(RIterOf, TIter, T)                                                 \
    static_assert(RIterOf<TIter, T>,                                                               \
        "{" #TIter "} does not satisfy {" #RIterOf "} requirement for type {" #T "}.")

#define _ATOM_SATISFIES_MUT_ITER(RMutIter, TIter)                                                  \
    static_assert(RMutIter<TIter>, "{" #TIter "} does not satisfy {" #RMutIter "} requirement.")

#define _ATOM_SATISFIES_MUT_ITER_OF(RMutIterOf, TIter, T)                                          \
    static_assert(RMutIterOf<TIter, T>,                                                            \
        "{" #TIter "} does not satisfy {" #RMutIterOf "} requirement for type {" #T "}.")

#define _ATOM_SATISFIES_ITER_PAIR(RIterPair, TIter, TIterEnd)                                      \
    static_assert(RIterPair<TIter, TIterEnd>,                                                      \
        "{" #TIter ", " #TIterEnd "} does not satisfy {" #RIterPair "} requirement.")

#define _ATOM_SATISFIES_ITER_PAIR_OF(RIterPairOf, TIter, TIterEnd, T)                              \
    static_assert(RIterPairOf<TIter, TIterEnd, T>,                                                 \
        "{" #TIter ", " #TIterEnd "} does not satisfy {" #RIterPairOf                              \
        "} requirement for type {" #T "}.")

#define _ATOM_SATISFIES_MUT_ITER_PAIR(RMutIterPair, TIter, TIterEnd)                               \
    static_assert(RMutIterPair<TIter, TIterEnd>,                                                   \
        "{" #TIter ", " #TIterEnd "} does not satisfy {" #RMutIterPair "} requirement.")

#define _ATOM_SATISFIES_MUT_ITER_PAIR_OF(RMutIterPairOf, TIter, TIterEnd, T)                       \
    static_assert(RMutIterPairOf<TIter, TIterEnd, T>,                                              \
        "{" #TIter ", " #TIterEnd "} does not satisfy {" #RMutIterPairOf                           \
        "} requirement for type {" #T "}.")

#define _ATOM_SATISFIES_ITER_TEMP(RIter, TIterTemp) _ATOM_SATISFIES_ITER(RIter, TIterTemp<i32>)

#define _ATOM_SATISFIES_MUT_ITER_TEMP(RIter, TIterTemp)                                            \
    _ATOM_SATISFIES_MUT_ITER(RIter, TIterTemp<i32>)

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region Iter Assertions

#define ATOM_SATISFIES_ITER(TIter) _ATOM_SATISFIES_ITER(RIter, TIter)
#define ATOM_SATISFIES_ITER_OF(TIter, T) _ATOM_SATISFIES_ITER_OF(RIterOf, TIter, T)
#define ATOM_SATISFIES_MUT_ITER(TIter) _ATOM_SATISFIES_MUT_ITER(RMutIter, TIter)
#define ATOM_SATISFIES_MUT_ITER_OF(TIter, T) _ATOM_SATISFIES_MUT_ITER_OF(RMutIterOf, TIter, T)

#define ATOM_SATISFIES_ITER_PAIR(TIter, TIterEnd)                                                  \
    _ATOM_SATISFIES_ITER_PAIR(RIterPair, TIter, TIterEnd)
#define ATOM_SATISFIES_ITER_PAIR_OF(TIter, TIterEnd, T)                                            \
    _ATOM_SATISFIES_ITER_PAIR_OF(RIterPairOf, TIter, TIterEnd, T)
#define ATOM_SATISFIES_MUT_ITER_PAIR(TIter, TIterEnd)                                              \
    _ATOM_SATISFIES_MUT_ITER_PAIR(RMutIterPair, TIter, TIterEnd)
#define ATOM_SATISFIES_MUT_ITER_PAIR_OF(TIter, TIterEnd, T)                                        \
    _ATOM_SATISFIES_MUT_ITER_PAIR_OF(RMutIterPairOf, TIter, TIterEnd, T)

#define ATOM_SATISFIES_ITER_TEMP(TIterTemp) _ATOM_SATISFIES_ITER_TEMP(RIter, TIterTemp)
#define ATOM_SATISFIES_MUT_ITER_TEMP(TIterTemp) _ATOM_SATISFIES_MUT_ITER_TEMP(RIter, TIterTemp)

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region Fwd Iter Assertions

#define ATOM_SATISFIES_FWD_ITER(TIter) _ATOM_SATISFIES_ITER(RFwdIter, TIter)
#define ATOM_SATISFIES_FWD_ITER_OF(TIter, T) _ATOM_SATISFIES_ITER_OF(RFwdIterOf, TIter, T)
#define ATOM_SATISFIES_MUT_FWD_ITER(TIter) _ATOM_SATISFIES_MUT_ITER(RMutFwdIter, TIter)
#define ATOM_SATISFIES_MUT_FWD_ITER_OF(TIter, T)                                                   \
    _ATOM_SATISFIES_MUT_ITER_OF(RMutFwdIterOf, TIter, T)

#define ATOM_SATISFIES_FWD_ITER_PAIR(TIter, TIterEnd)                                              \
    _ATOM_SATISFIES_ITER_PAIR(RFwdIterPair, TIter, TIterEnd)
#define ATOM_SATISFIES_FWD_ITER_PAIR_OF(TIter, TIterEnd, T)                                        \
    _ATOM_SATISFIES_ITER_PAIR_OF(RFwdIterPairOf, TIter, TIterEnd, T)
#define ATOM_SATISFIES_MUT_FWD_ITER_PAIR(TIter, TIterEnd)                                          \
    _ATOM_SATISFIES_MUT_ITER_PAIR(RMutFwdIterPair, TIter, TIterEnd)
#define ATOM_SATISFIES_MUT_FWD_ITER_PAIR_OF(TIter, TIterEnd, T)                                    \
    _ATOM_SATISFIES_MUT_ITER_PAIR_OF(RMutFwdIterPairOf, TIter, TIterEnd, T)

#define ATOM_SATISFIES_FWD_ITER_TEMP(TIterTemp) _ATOM_SATISFIES_ITER_TEMP(RFwdIter, TIterTemp)
#define ATOM_SATISFIES_MUT_FWD_ITER_TEMP(TIterTemp)                                                \
    _ATOM_SATISFIES_MUT_ITER_TEMP(RFwdIter, TIterTemp)

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region Bidi Iter Assertions

#define ATOM_SATISFIES_BIDI_ITER(TIter) _ATOM_SATISFIES_ITER(RBidiIter, TIter)
#define ATOM_SATISFIES_BIDI_ITER_OF(TIter, T) _ATOM_SATISFIES_ITER_OF(RBidiIterOf, TIter, T)
#define ATOM_SATISFIES_MUT_BIDI_ITER(TIter) _ATOM_SATISFIES_MUT_ITER(RMutBidiIter, TIter)
#define ATOM_SATISFIES_MUT_BIDI_ITER_OF(TIter, T)                                                  \
    _ATOM_SATISFIES_MUT_ITER_OF(RMutBidiIterOf, TIter, T)

#define ATOM_SATISFIES_BIDI_ITER_PAIR(TIter, TIterEnd)                                             \
    _ATOM_SATISFIES_ITER_PAIR(RBidiIterPair, TIter, TIterEnd)
#define ATOM_SATISFIES_BIDI_ITER_PAIR_OF(TIter, TIterEnd, T)                                       \
    _ATOM_SATISFIES_ITER_PAIR_OF(RBidiIterPairOf, TIter, TIterEnd, T)
#define ATOM_SATISFIES_MUT_BIDI_ITER_PAIR(TIter, TIterEnd)                                         \
    _ATOM_SATISFIES_MUT_ITER_PAIR(RMutBidiIterPair, TIter, TIterEnd)
#define ATOM_SATISFIES_MUT_BIDI_ITER_PAIR_OF(TIter, TIterEnd, T)                                   \
    _ATOM_SATISFIES_MUT_ITER_PAIR_OF(RMutBidiIterPairOf, TIter, TIterEnd, T)

#define ATOM_SATISFIES_BIDI_ITER_TEMP(TIterTemp) _ATOM_SATISFIES_ITER_TEMP(RBidiIter, TIterTemp)
#define ATOM_SATISFIES_MUT_BIDI_ITER_TEMP(TIterTemp)                                               \
    _ATOM_SATISFIES_MUT_ITER_TEMP(RBidiIter, TIterTemp)

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region Jump Iter Assertions

#define ATOM_SATISFIES_JUMP_ITER(TIter) _ATOM_SATISFIES_ITER(RJumpIter, TIter)
#define ATOM_SATISFIES_JUMP_ITER_OF(TIter, T) _ATOM_SATISFIES_ITER_OF(RJumpIterOf, TIter, T)
#define ATOM_SATISFIES_MUT_JUMP_ITER(TIter) _ATOM_SATISFIES_MUT_ITER(RMutJumpIter, TIter)
#define ATOM_SATISFIES_MUT_JUMP_ITER_OF(TIter, T)                                                  \
    _ATOM_SATISFIES_MUT_ITER_OF(RMutJumpIterOf, TIter, T)

#define ATOM_SATISFIES_JUMP_ITER_PAIR(TIter, TIterEnd)                                             \
    _ATOM_SATISFIES_ITER_PAIR(RJumpIterPair, TIter, TIterEnd)
#define ATOM_SATISFIES_JUMP_ITER_PAIR_OF(TIter, TIterEnd, T)                                       \
    _ATOM_SATISFIES_ITER_PAIR_OF(RJumpIterPairOf, TIter, TIterEnd, T)
#define ATOM_SATISFIES_MUT_JUMP_ITER_PAIR(TIter, TIterEnd)                                         \
    _ATOM_SATISFIES_MUT_ITER_PAIR(RMutJumpIterPair, TIter, TIterEnd)
#define ATOM_SATISFIES_MUT_JUMP_ITER_PAIR_OF(TIter, TIterEnd, T)                                   \
    _ATOM_SATISFIES_MUT_ITER_PAIR_OF(RMutJumpIterPairOf, TIter, TIterEnd, T)

#define ATOM_SATISFIES_JUMP_ITER_TEMP(TIterTemp) _ATOM_SATISFIES_ITER_TEMP(RJumpIter, TIterTemp)
#define ATOM_SATISFIES_MUT_JUMP_ITER_TEMP(TIterTemp)                                               \
    _ATOM_SATISFIES_MUT_ITER_TEMP(RJumpIter, TIterTemp)

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region Arr Iter Assertions

#define ATOM_SATISFIES_ARR_ITER(TIter) _ATOM_SATISFIES_ITER(RArrIter, TIter)
#define ATOM_SATISFIES_ARR_ITER_OF(TIter, T) _ATOM_SATISFIES_ITER_OF(RArrIterOf, TIter, T)
#define ATOM_SATISFIES_MUT_ARR_ITER(TIter) _ATOM_SATISFIES_MUT_ITER(RMutArrIter, TIter)
#define ATOM_SATISFIES_MUT_ARR_ITER_OF(TIter, T)                                                   \
    _ATOM_SATISFIES_MUT_ITER_OF(RMutArrIterOf, TIter, T)

#define ATOM_SATISFIES_ARR_ITER_PAIR(TIter, TIterEnd)                                              \
    _ATOM_SATISFIES_ITER_PAIR(RArrIterPair, TIter, TIterEnd)
#define ATOM_SATISFIES_ARR_ITER_PAIR_OF(TIter, TIterEnd, T)                                        \
    _ATOM_SATISFIES_ITER_PAIR_OF(RArrIterPairOf, TIter, TIterEnd, T)
#define ATOM_SATISFIES_MUT_ARR_ITER_PAIR(TIter, TIterEnd)                                          \
    _ATOM_SATISFIES_MUT_ITER_PAIR(RMutArrIterPair, TIter, TIterEnd)
#define ATOM_SATISFIES_MUT_ARR_ITER_PAIR_OF(TIter, TIterEnd, T)                                    \
    _ATOM_SATISFIES_MUT_ITER_PAIR_OF(RMutArrIterPairOf, TIter, TIterEnd, T)

#define ATOM_SATISFIES_ARR_ITER_TEMP(TIterTemp) _ATOM_SATISFIES_ITER_TEMP(RArrIter, TIterTemp)
#define ATOM_SATISFIES_MUT_ARR_ITER_TEMP(TIterTemp)                                                \
    _ATOM_SATISFIES_MUT_ITER_TEMP(RArrIter, TIterTemp)

#pragma endregion
//// -----------------------------------------------------------------------------------------------
