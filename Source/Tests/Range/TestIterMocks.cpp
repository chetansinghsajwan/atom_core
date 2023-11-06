#include "Atom/Range/IterReqMocks.h"
#include "Range/IterReqChecks.h"

using namespace Atom;

ATOM_SATISFIES_ITER_PAIR(IterReqMock<i32>, IterEndReqMock);
ATOM_SATISFIES_MUT_ITER_PAIR(MutIterReqMock<i32>, IterEndReqMock);
ATOM_SATISFIES_FWD_ITER_PAIR(FwdIterReqMock<i32>, IterEndReqMock);
ATOM_SATISFIES_MUT_FWD_ITER_PAIR(MutFwdIterReqMock<i32>, IterEndReqMock);
ATOM_SATISFIES_BIDI_ITER_PAIR(BidiIterReqMock<i32>, IterEndReqMock);
ATOM_SATISFIES_MUT_BIDI_ITER_PAIR(MutBidiIterReqMock<i32>, IterEndReqMock);
ATOM_SATISFIES_JUMP_ITER_PAIR(JumpIterReqMock<i32>, IterEndReqMock);
ATOM_SATISFIES_MUT_JUMP_ITER_PAIR(MutJumpIterReqMock<i32>, IterEndReqMock);
ATOM_SATISFIES_ARR_ITER_PAIR(ArrayIterReqMock<i32>, ArrayIterReqMock<i32>);
ATOM_SATISFIES_MUT_ARR_ITER_PAIR(MutArrayIterReqMock<i32>, MutArrayIterReqMock<i32>);
