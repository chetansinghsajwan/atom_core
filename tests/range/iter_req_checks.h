#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// iter assertion implementation
////
////////////////////////////////////////////////////////////////////////////////////////////////////

#define _atom_satisfies_iter(riter, iter_type)                                                         \
    static_assert(riter<iter_type>, "{" #iter_type "} does not satisfy {" #riter "} requirement.")

#define _atom_satisfies_iter_of(riter_of, iter_type, type)                                                 \
    static_assert(riter_of<iter_type, type>,                                                               \
        "{" #iter_type "} does not satisfy {" #riter_of "} requirement for type {" #type "}.")

#define _atom_satisfies_mut_iter(rmut_iter, iter_type)                                                  \
    static_assert(rmut_iter<iter_type>, "{" #iter_type "} does not satisfy {" #rmut_iter "} requirement.")

#define _atom_satisfies_mut_iter_of(rmut_iter_of, iter_type, type)                                          \
    static_assert(rmut_iter_of<iter_type, type>,                                                            \
        "{" #iter_type "} does not satisfy {" #rmut_iter_of "} requirement for type {" #type "}.")

#define _atom_satisfies_iter_pair(riter_pair, iter_type, iter_end_type)                                      \
    static_assert(riter_pair<iter_type, iter_end_type>,                                                      \
        "{" #iter_type ", " #iter_end_type "} does not satisfy {" #riter_pair "} requirement.")

#define _atom_satisfies_iter_pair_of(riter_pair_of, iter_type, iter_end_type, type)                              \
    static_assert(riter_pair_of<iter_type, iter_end_type, type>,                                                 \
        "{" #iter_type ", " #iter_end_type "} does not satisfy {" #riter_pair_of                              \
        "} requirement for type {" #type "}.")

#define _atom_satisfies_mut_iter_pair(rmut_iter_pair, iter_type, iter_end_type)                               \
    static_assert(rmut_iter_pair<iter_type, iter_end_type>,                                                   \
        "{" #iter_type ", " #iter_end_type "} does not satisfy {" #rmut_iter_pair "} requirement.")

#define _atom_satisfies_mut_iter_pair_of(rmut_iter_pair_of, iter_type, iter_end_type, type)                       \
    static_assert(rmut_iter_pair_of<iter_type, iter_end_type, type>,                                              \
        "{" #iter_type ", " #iter_end_type "} does not satisfy {" #rmut_iter_pair_of                           \
        "} requirement for type {" #type "}.")

#define _atom_satisfies_iter_temp(riter, titer_temp) _atom_satisfies_iter(riter, titer_temp<i32>)

#define _atom_satisfies_mut_iter_temp(riter, titer_temp)                                            \
    _atom_satisfies_mut_iter(riter, titer_temp<i32>)

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// iter assertions
////
////////////////////////////////////////////////////////////////////////////////////////////////////

#define atom_satisfies_iter(iter_type) _atom_satisfies_iter(riter, iter_type)
#define atom_satisfies_iter_of(iter_type, type) _atom_satisfies_iter_of(riter_of, iter_type, type)
#define atom_satisfies_mut_iter(iter_type) _atom_satisfies_mut_iter(rmut_iter, iter_type)
#define atom_satisfies_mut_iter_of(iter_type, type) _atom_satisfies_mut_iter_of(rmut_iter_of, iter_type, type)

#define atom_satisfies_iter_pair(iter_type, iter_end_type)                                                  \
    _atom_satisfies_iter_pair(riter_pair, iter_type, iter_end_type)
#define atom_satisfies_iter_pair_of(iter_type, iter_end_type, type)                                            \
    _atom_satisfies_iter_pair_of(riter_pair_of, iter_type, iter_end_type, type)
#define atom_satisfies_mut_iter_pair(iter_type, iter_end_type)                                              \
    _atom_satisfies_mut_iter_pair(rmut_iter_pair, iter_type, iter_end_type)
#define atom_satisfies_mut_iter_pair_of(iter_type, iter_end_type, type)                                        \
    _atom_satisfies_mut_iter_pair_of(rmut_iter_pair_of, iter_type, iter_end_type, type)

#define atom_satisfies_iter_temp(titer_temp) _atom_satisfies_iter_temp(riter, titer_temp)
#define atom_satisfies_mut_iter_temp(titer_temp) _atom_satisfies_mut_iter_temp(riter, titer_temp)

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// fwd iter assertions
////
////////////////////////////////////////////////////////////////////////////////////////////////////

#define atom_satisfies_fwd_iter(iter_type) _atom_satisfies_iter(rfwd_iter, iter_type)
#define atom_satisfies_fwd_iter_of(iter_type, type) _atom_satisfies_iter_of(rfwd_iter_of, iter_type, type)
#define atom_satisfies_mut_fwd_iter(iter_type) _atom_satisfies_mut_iter(rmut_fwd_iter, iter_type)
#define atom_satisfies_mut_fwd_iter_of(iter_type, type)                                                   \
    _atom_satisfies_mut_iter_of(rmut_fwd_iter_of, iter_type, type)

#define atom_satisfies_fwd_iter_pair(iter_type, iter_end_type)                                              \
    _atom_satisfies_iter_pair(rfwd_iter_pair, iter_type, iter_end_type)
#define atom_satisfies_fwd_iter_pair_of(iter_type, iter_end_type, type)                                        \
    _atom_satisfies_iter_pair_of(rfwd_iter_pair_of, iter_type, iter_end_type, type)
#define atom_satisfies_mut_fwd_iter_pair(iter_type, iter_end_type)                                          \
    _atom_satisfies_mut_iter_pair(rmut_fwd_iter_pair, iter_type, iter_end_type)
#define atom_satisfies_mut_fwd_iter_pair_of(iter_type, iter_end_type, type)                                    \
    _atom_satisfies_mut_iter_pair_of(rmut_fwd_iter_pair_of, iter_type, iter_end_type, type)

#define atom_satisfies_fwd_iter_temp(titer_temp) _atom_satisfies_iter_temp(rfwd_iter, titer_temp)
#define atom_satisfies_mut_fwd_iter_temp(titer_temp)                                                \
    _atom_satisfies_mut_iter_temp(rfwd_iter, titer_temp)

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// bidi iter assertions
////
////////////////////////////////////////////////////////////////////////////////////////////////////

#define atom_satisfies_bidi_iter(iter_type) _atom_satisfies_iter(rbidi_iter, iter_type)
#define atom_satisfies_bidi_iter_of(iter_type, type) _atom_satisfies_iter_of(rbidi_iter_of, iter_type, type)
#define atom_satisfies_mut_bidi_iter(iter_type) _atom_satisfies_mut_iter(rmut_bidi_iter, iter_type)
#define atom_satisfies_mut_bidi_iter_of(iter_type, type)                                                  \
    _atom_satisfies_mut_iter_of(rmut_bidi_iter_of, iter_type, type)

#define atom_satisfies_bidi_iter_pair(iter_type, iter_end_type)                                             \
    _atom_satisfies_iter_pair(rbidi_iter_pair, iter_type, iter_end_type)
#define atom_satisfies_bidi_iter_pair_of(iter_type, iter_end_type, type)                                       \
    _atom_satisfies_iter_pair_of(rbidi_iter_pair_of, iter_type, iter_end_type, type)
#define atom_satisfies_mut_bidi_iter_pair(iter_type, iter_end_type)                                         \
    _atom_satisfies_mut_iter_pair(rmut_bidi_iter_pair, iter_type, iter_end_type)
#define atom_satisfies_mut_bidi_iter_pair_of(iter_type, iter_end_type, type)                                   \
    _atom_satisfies_mut_iter_pair_of(rmut_bidi_iter_pair_of, iter_type, iter_end_type, type)

#define atom_satisfies_bidi_iter_temp(titer_temp) _atom_satisfies_iter_temp(rbidi_iter, titer_temp)
#define atom_satisfies_mut_bidi_iter_temp(titer_temp)                                               \
    _atom_satisfies_mut_iter_temp(rbidi_iter, titer_temp)

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// jump iter assertions
////
////////////////////////////////////////////////////////////////////////////////////////////////////

#define atom_satisfies_jump_iter(iter_type) _atom_satisfies_iter(rjump_iter, iter_type)
#define atom_satisfies_jump_iter_of(iter_type, type) _atom_satisfies_iter_of(rjump_iter_of, iter_type, type)
#define atom_satisfies_mut_jump_iter(iter_type) _atom_satisfies_mut_iter(rmut_jump_iter, iter_type)
#define atom_satisfies_mut_jump_iter_of(iter_type, type)                                                  \
    _atom_satisfies_mut_iter_of(rmut_jump_iter_of, iter_type, type)

#define atom_satisfies_jump_iter_pair(iter_type, iter_end_type)                                             \
    _atom_satisfies_iter_pair(rjump_iter_pair, iter_type, iter_end_type)
#define atom_satisfies_jump_iter_pair_of(iter_type, iter_end_type, type)                                       \
    _atom_satisfies_iter_pair_of(rjump_iter_pair_of, iter_type, iter_end_type, type)
#define atom_satisfies_mut_jump_iter_pair(iter_type, iter_end_type)                                         \
    _atom_satisfies_mut_iter_pair(rmut_jump_iter_pair, iter_type, iter_end_type)
#define atom_satisfies_mut_jump_iter_pair_of(iter_type, iter_end_type, type)                                   \
    _atom_satisfies_mut_iter_pair_of(rmut_jump_iter_pair_of, iter_type, iter_end_type, type)

#define atom_satisfies_jump_iter_temp(titer_temp) _atom_satisfies_iter_temp(rjump_iter, titer_temp)
#define atom_satisfies_mut_jump_iter_temp(titer_temp)                                               \
    _atom_satisfies_mut_iter_temp(rjump_iter, titer_temp)

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// array iter assertions
////
////////////////////////////////////////////////////////////////////////////////////////////////////

#define atom_satisfies_arr_iter(iter_type) _atom_satisfies_iter(rarray_iter, iter_type)
#define atom_satisfies_arr_iter_of(iter_type, type) _atom_satisfies_iter_of(rarray_iter_of, iter_type, type)
#define atom_satisfies_mut_arr_iter(iter_type) _atom_satisfies_mut_iter(rmut_array_iter, iter_type)
#define atom_satisfies_mut_arr_iter_of(iter_type, type)                                                   \
    _atom_satisfies_mut_iter_of(rmut_array_iter_of, iter_type, type)

#define atom_satisfies_arr_iter_pair(iter_type, iter_end_type)                                              \
    _atom_satisfies_iter_pair(rarray_iter_pair, iter_type, iter_end_type)
#define atom_satisfies_arr_iter_pair_of(iter_type, iter_end_type, type)                                        \
    _atom_satisfies_iter_pair_of(rarray_iter_pair_of, iter_type, iter_end_type, type)
#define atom_satisfies_mut_arr_iter_pair(iter_type, iter_end_type)                                          \
    _atom_satisfies_mut_iter_pair(rmut_array_iter_pair, iter_type, iter_end_type)
#define atom_satisfies_mut_arr_iter_pair_of(iter_type, iter_end_type, type)                                    \
    _atom_satisfies_mut_iter_pair_of(rmut_array_iter_pair_of, iter_type, iter_end_type, type)

#define atom_satisfies_arr_iter_temp(titer_temp) _atom_satisfies_iter_temp(rarray_iter, titer_temp)
#define atom_satisfies_mut_arr_iter_temp(titer_temp)                                                \
    _atom_satisfies_mut_iter_temp(rarray_iter, titer_temp)
