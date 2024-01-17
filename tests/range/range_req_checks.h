#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// implementations
////
////////////////////////////////////////////////////////////////////////////////////////////////////

#define _atom_satisfies_range(rrange, range_type)                                                      \
    static_assert(rrange<range_type>, "{" #range_type "} does not satisfy {" #rrange "} requirements.");

#define _atom_satisfies_range_of(rrange, range_type, type)                                                \
    static_assert(rrange<range_type, type>,                                                               \
        "{" #range_type "} does not satisfy {" #rrange "} requirements for type {" #type "}.");

#define _atom_satisfies_range_temp(rrange, range_type)                                                 \
    static_assert(                                                                                 \
        rrange<range_type<i32>>, "{" #range_type "} does not satisfy {" #rrange "} requirements.");

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// checks
////
////////////////////////////////////////////////////////////////////////////////////////////////////

#define atom_satisfies_range(range_type) _atom_satisfies_range(rrange, range_type)
#define atom_satisfies_range_of(range_type, type) _atom_satisfies_range_of(rrange_of, range_type, type)
#define atom_satisfies_range_temp(range_type) _atom_satisfies_range_temp(rrange, range_type)

#define atom_satisfies_mut_range(range_type) _atom_satisfies_range(rmut_range, range_type)
#define atom_satisfies_mut_range_of(range_type, type) _atom_satisfies_range_of(rmut_range_of, range_type, type)
#define atom_satisfies_mut_range_temp(range_type) _atom_satisfies_range_temp(rmut_range, range_type)

#define atom_satisfies_fwd_range(range_type) _atom_satisfies_range(rfwd_range, range_type)
#define atom_satisfies_fwd_range_of(range_type, type) _atom_satisfies_range_of(rfwd_range_of, range_type, type)
#define atom_satisfies_fwd_range_temp(range_type) _atom_satisfies_range_temp(rfwd_range, range_type)

#define atom_satisfies_mut_fwd_range(range_type) _atom_satisfies_range(rmut_fwd_range, range_type)
#define atom_satisfies_mut_fwd_range_of(range_type, type)                                                 \
    _atom_satisfies_range_of(rmut_fwd_range_of, range_type, type)
#define atom_satisfies_mut_fwd_range_temp(range_type) _atom_satisfies_range_temp(rmut_fwd_range, range_type)

#define atom_satisfies_bidi_range(range_type) _atom_satisfies_range(rbidi_range, range_type)
#define atom_satisfies_bidi_range_of(range_type, type) _atom_satisfies_range_of(rbidi_range_of, range_type, type)
#define atom_satisfies_bidi_range_temp(range_type) _atom_satisfies_range_temp(rbidi_range, range_type)

#define atom_satisfies_mut_bidi_range(range_type) _atom_satisfies_range(rmut_bidi_range, range_type)
#define atom_satisfies_mut_bidi_range_of(range_type, type)                                                \
    _atom_satisfies_range_of(rmut_bidi_range_of, range_type, type)
#define atom_satisfies_mut_bidi_range_temp(range_type) _atom_satisfies_range_temp(rmut_bidi_range, range_type)

#define atom_satisfies_jump_range(range_type) _atom_satisfies_range(rjump_range, range_type)
#define atom_satisfies_jump_range_of(range_type, type) _atom_satisfies_range_of(rjump_range_of, range_type, type)
#define atom_satisfies_jump_range_temp(range_type) _atom_satisfies_range_temp(rjump_range, range_type)

#define atom_satisfies_mut_jump_range(range_type) _atom_satisfies_range(rmut_jump_range, range_type)
#define atom_satisfies_mut_jump_range_of(range_type, type)                                                \
    _atom_satisfies_range_of(rmut_jump_range_of, range_type, type)
#define atom_satisfies_mut_jump_range_temp(range_type) _atom_satisfies_range_temp(rmut_jump_range, range_type)

#define atom_satisfies_arr_range(range_type) _atom_satisfies_range(rarray_range, range_type)
#define atom_satisfies_arr_range_of(range_type, type) _atom_satisfies_range_of(rarray_range_of, range_type, type)
#define atom_satisfies_arr_range_temp(range_type) _atom_satisfies_range_temp(rarray_range, range_type)

#define atom_satisfies_mut_arr_range(range_type) _atom_satisfies_range(rmut_array_range, range_type)
#define atom_satisfies_mut_arr_range_of(range_type, type)                                                 \
    _atom_satisfies_range_of(rmut_array_range_of, range_type, type)
#define atom_satisfies_mut_arr_range_temp(range_type) _atom_satisfies_range_temp(rmut_array_range, range_type)

#define atom_satisfies_rev_range(range_type) _atom_satisfies_range(rrev_range, range_type)
#define atom_satisfies_rev_range_of(range_type, type) _atom_satisfies_range_of(rrev_range_of, range_type, type)
#define atom_satisfies_rev_range_temp(range_type) _atom_satisfies_range_temp(rrev_range, range_type)

#define atom_satisfies_mut_rev_range(range_type) _atom_satisfies_range(rmut_rev_range, range_type)
#define atom_satisfies_mut_rev_range_of(range_type, type)                                                 \
    _atom_satisfies_range_of(rmut_rev_range_of, range_type, type)
#define atom_satisfies_mut_rev_range_temp(range_type) _atom_satisfies_range_temp(rmut_rev_range, range_type)

#define atom_satisfies_common_range(range_type) _atom_satisfies_range(rcommon_range, range_type)
#define atom_satisfies_common_range_of(range_type, type)                                                  \
    _atom_satisfies_range_of(rcommon_range_of, range_type, type)
#define atom_satisfies_common_range_temp(range_type) _atom_satisfies_range_temp(rcommon_range, range_type)

#define atom_satisfies_mut_common_range(range_type) _atom_satisfies_range(rmut_common_range, range_type)
#define atom_satisfies_mut_common_range_temp(range_type)                                               \
    _atom_satisfies_range_temp(rmut_common_range, range_type)
