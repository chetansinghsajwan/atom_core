#pragma once

//// Possible Compiler Values
//// -----------------------------------------------------------------------------------------------
#define ATOM_COMPILER_MSVC  1
#define ATOM_COMPILER_GCC   2
#define ATOM_COMPILER_CLANG 3

//// MSVC
//// -----------------------------------------------------------------------------------------------
#if defined(_MSC_VER)

    #if defined(ATOM_COMPILER)
        #error ATOM_COMPILER already defined.

    #endif

    #define ATOM_COMPILER           ATOM_COMPILER_MSVC
    #define ATOM_COMPILER_NAME      "MSVC"
    #define ATOM_COMPILER_VER       _MSC_VER
    #define ATOM_COMPILER_FULL_VER  _MSC_FULL_VER
    #define ATOM_COMPILER_VER_MAJOR (_MSC_VER / 100)
    #define ATOM_COMPILER_VER_MINOR (_MSC_VER % 100)
    #define ATOM_COMPILER_VER_PATCH (_MSC_VER)
    #define ATOM_COMPILER_VER_BUILD _MSC_BUILD

#endif

//// GNUC
//// -----------------------------------------------------------------------------------------------
#if defined(__GNUC__)

    #if defined(ATOM_COMPILER)
        #error ATOM_COMPILER already defined.

    #endif

    #define ATOM_COMPILER           ATOM_COMPILER_GCC
    #define ATOM_COMPILER_NAME      "GNUC"
    #define ATOM_COMPILER_VER       (__GNUC__ * 100) + __GNUC_MINOR__
    #define ATOM_COMPILER_FULL_VER  (ATOM_COMPILER_VER * 100) + __GNUC_PATCHLEVEL__
    #define ATOM_COMPILER_VER_MAJOR __GNUC__
    #define ATOM_COMPILER_VER_MINOR __GNUC_MINOR__
    #define ATOM_COMPILER_VER_PATCH __GNUC_PATCHLEVEL__
    #define ATOM_COMPILER_VER_BUILD 0

#endif

//// Clang
//// -----------------------------------------------------------------------------------------------
#if defined(__clang__)

    #if defined(ATOM_COMPILER)
        #error ATOM_COMPILER already defined.

    #endif

    #define ATOM_COMPILER           ATOM_COMPILER_CLANG
    #define ATOM_COMPILER_NAME      "CLANG"
    #define ATOM_COMPILER_VER       __clang_version__
    #define ATOM_COMPILER_FULL_VER  __clang_version__
    #define ATOM_COMPILER_VER_MAJOR __clang_major__
    #define ATOM_COMPILER_VER_MINOR __clang_minor__
    #define ATOM_COMPILER_VER_PATCH __clang_patchlevel__
    #define ATOM_COMPILER_VER_BUILD 0

#endif

//// Fail Check
//// -----------------------------------------------------------------------------------------------
#if !defined(ATOM_COMPILER)
    #error Could not determine value for ATOM_COMPILER.

#endif