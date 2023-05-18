#pragma once

//// Possible Compiler Values
//// -----------------------------------------------------------------------------------------------
#define ATOM_COMP_NULL_ID  0
#define ATOM_COMP_CLANG_ID 1
#define ATOM_COMP_GCC_ID   2
#define ATOM_COMP_MSVC_ID  3

//// Clang
//// -----------------------------------------------------------------------------------------------
#if defined(__clang__)

    #define ATOM_COMP_ID              ATOM_COMP_CLANG_ID
    #define ATOM_COMP_NAME            "Clang"

    #define ATOM_COMP_CLANG           1
    #define ATOM_COMP_CLANG_VER_MAJOR __clang_major__
    #define ATOM_COMP_CLANG_VER_MINOR __clang_minor__
    #define ATOM_COMP_CLANG_VER_PATCH __clang_patchlevel_
    #define ATOM_COMP_CLANG_VER       __clang_version__

#endif

//// MSVC
//// -----------------------------------------------------------------------------------------------
#if defined(_MSC_VER)

    #ifdef ATOM_COMP_ID
        #define ATOM_COMP_MSVC_EMUL
    #else
        #define ATOM_COMP_ID   ATOM_COMP_MSVC_ID
        #define ATOM_COMP_NAME "MSVC"

        #define ATOM_COMP_MSVC
    #endif

    #define ATOM_COMP_MSVC_VER_MAJOR (_MSC_VER / 100)
    #define ATOM_COMP_MSVC_VER_MINOR (_MSC_VER % 100)
    #define ATOM_COMP_MSVC_VER_PATCH (_MSC_VER)
    #define ATOM_COMP_MSVC_VER       _MSC_VER

#endif

//// GNUC
//// -----------------------------------------------------------------------------------------------
#if defined(__GNUC__)

    #ifdef ATOM_COMP_ID
        #define ATOM_COMP_GNUC_EMUL
    #else
        #define ATOM_COMP_ID   ATOM_COMP_GNUC_ID
        #define ATOM_COMP_NAME "GNUC"

        #define ATOM_COMP_GNUC
    #endif

    #define ATOM_COMP_GNUC_VER_MAJOR __GNUC__
    #define ATOM_COMP_GNUC_VER_MINOR __GNUC_MINOR__
    #define ATOM_COMP_GNUC_VER_PATCH __GNUC_PATCHLEVEL__
    #define ATOM_COMP_GNUC_VER       (__GNUC__ * 100) + __GNUC_MINOR__

#endif

//// Fail Check
//// -----------------------------------------------------------------------------------------------
#if !defined(ATOM_COMP_ID)
    #error Could not determine value for ATOM_COMP_ID.

#endif