#pragma once
#include "Atom/Core/Platform.h"
#include "Atom/Core/Compiler.h"
#include "Atom/Core/Requirements.h"
#include "Atom/Core/Primitives.h"

//// -----------------------------------------------------------------------------------------------
//// ATOM API
//// -----------------------------------------------------------------------------------------------

#if ATOM_PLATFORM == ATOM_PLATFORM_WINDOWS
    #ifdef ATOM_EXPORT
        #define ATOM_API __declspec(dllexport)
    #else
        #define ATOM_API __declspec(dllimport)
    #endif

#elif ATOM_PLATFORM == ATOM_PLATFORM_LINUX
    #ifdef ATOM_EXPORT
        #define ATOM_API __attribute__((visibility("default")))
    #else
        #define ATOM_API
    #endif

#else
    #error "AtomEngine only supports Windows and Linux platform for now."

#endif
