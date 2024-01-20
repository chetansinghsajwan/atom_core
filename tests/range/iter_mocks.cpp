import atom.core;
#include "range.h"

using namespace atom;

atom_satisfies_iter_pair(iter_req_mock<i32>, iter_end_req_mock);
atom_satisfies_mut_iter_pair(mut_iter_req_mock<i32>, iter_end_req_mock);
atom_satisfies_fwd_iter_pair(fwd_iter_req_mock<i32>, iter_end_req_mock);
atom_satisfies_mut_fwd_iter_pair(mut_fwd_iter_req_mock<i32>, iter_end_req_mock);
atom_satisfies_bidi_iter_pair(bidi_iter_req_mock<i32>, iter_end_req_mock);
atom_satisfies_mut_bidi_iter_pair(mut_bidi_iter_req_mock<i32>, iter_end_req_mock);
atom_satisfies_jump_iter_pair(jump_iter_req_mock<i32>, iter_end_req_mock);
atom_satisfies_mut_jump_iter_pair(mut_jump_iter_req_mock<i32>, iter_end_req_mock);
atom_satisfies_arr_iter_pair(array_iter_req_mock<i32>, array_iter_req_mock<i32>);
atom_satisfies_mut_arr_iter_pair(mut_array_iter_req_mock<i32>, mut_array_iter_req_mock<i32>);
