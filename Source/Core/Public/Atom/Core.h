#pragma once
#include "Core/Compiler.h"
#include "Core/LangExtensions.h"
#include "Core/Platform.h"
#include "Core/Primitives.h"
#include "Core/Requirements.h"

//// -----------------------------------------------------------------------------------------------
//// ATOM API
//// -----------------------------------------------------------------------------------------------

#if ATOM_PLATFORM == ATOM_PLATFORM_WINDOWS
#    ifdef ATOM_EXPORT
#        define ATOM_API __declspec(dllexport)
#    else
#        define ATOM_API __declspec(dllimport)
#    endif

#elif ATOM_PLATFORM == ATOM_PLATFORM_LINUX
#    ifdef ATOM_EXPORT
#        define ATOM_API __attribute__((visibility("default")))
#    else
#        define ATOM_API
#    endif

#else
#    error "AtomEngine only supports Windows and Linux platform for now."

#endif

//// -----------------------------------------------------------------------------------------------
//// Assertions
//// -----------------------------------------------------------------------------------------------

#define expects(...)
#define asserts(...)
#define ensures(...)
#define debug_expects(...)
#define debug_asserts(...)
#define debug_ensures(...)
#define fnret 0
#define static_expects(...) static_assert(__VA_ARGS__)
#define panic(...) myTerminate(__VA_ARGS__)
