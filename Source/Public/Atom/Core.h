#pragma once
#include "Core/Compiler.h"
#include "Core/LangExtensions.h"
#include "Core/Platform.h"
#include "Core/Primitives.h"
#include "Core/Requirements.h"

/// ------------------------------------------------------------------------------------------------
/// ATOM API
/// ------------------------------------------------------------------------------------------------
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

/// ------------------------------------------------------------------------------------------------
/// Optimization handling macros.
///
/// # Macros
/// - `ATOM_PRAGMA_OPTIMIZE_OFF`: Disables all optimization until [`ATOM_PRAGMA_OPTIMIZE_ON`] is
///   encountered.
/// - `ATOM_PRAGMA_OPTIMIZE_ON`: Enables optimizations disabled by [`ATOM_PRAGMA_OPTIMIZE_OFF`].
/// ------------------------------------------------------------------------------------------------
#if defined(ATOM_COMP_MSVC)
#    define ATOM_PRAGMA_OPTIMIZE_OFF _Pragma("optimize(\"\", off)");
#    define ATOM_PRAGMA_OPTIMIZE_ON _Pragma("optimize(\"\", on)");
#elif defined(ATOM_COMP_GNUC)
#    define ATOM_PRAGMA_OPTIMIZE_OFF _Pragma("GCC push_options") _Pragma("GCC optimize(\"O0\")")
#    define ATOM_PRAGMA_OPTIMIZE_ON _Pragma("GCC pop_options")
#elif defined(ATOM_COMP_CLANG)
#    define ATOM_PRAGMA_OPTIMIZE_OFF _Pragma("clang optimize off")
#    define ATOM_PRAGMA_OPTIMIZE_ON _Pragma("clang optimize on")
#endif
