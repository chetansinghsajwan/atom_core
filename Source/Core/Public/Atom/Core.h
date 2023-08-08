#pragma once
#include "Core/Platform.h"
#include "Core/Compiler.h"
#include "Core/Requirements.h"
#include "Core/Primitives.h"
#include "Core/LangExtensions.h"

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

//// -----------------------------------------------------------------------------------------------
//// Assertions
//// -----------------------------------------------------------------------------------------------

#define expects(...)
#define debug_expects(...)
#define debug_asserts(...)
#define panic(...) myTerminate(__VA_ARGS__)
