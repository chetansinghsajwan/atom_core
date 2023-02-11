#pragma once

#if defined(ATOM_PLATFORM_WIN)
    #ifdef ATOM_EXPORT
        #define ATOM_API __declspec(dllexport)
    #else
        #define ATOM_API __declspec(dllimport)
    #endif

#elif defined(ATOM_PLATFORM_LINUX)
    #ifdef ATOM_EXPORT
        #define ATOM_API __attribute__((visibility("default")))
    #endif

#else
    #define ATOM_API
    // #error "AtomEngine only supports Windows and Linux platform."

#endif