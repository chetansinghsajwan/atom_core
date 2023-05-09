#pragma once
// #include "Atom/String/UTF8CharEncodingConverters.h"

#include "Atom/Exceptions/Exceptions.decl.h"

//// libassert [BEGIN]
//// -----------------------------------------------------------------------------------------------

// Custom fail function for libassert.
#define ASSERT_FAIL _Atom_libassert_CustomFailAction

#include "assert.hpp"

// Undefine libassert macros, we use our custom ATOM_ macros.
#undef DEBUG_ASSERT
#undef ASSERT
#undef ASSUME
#undef VERIFY

//// libassert [END]
//// -----------------------------------------------------------------------------------------------

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    class AssertionException: public Exception
    {
    public:
        AssertionException(const char16* in_msg) noexcept;
    };
}

/// 
/// ------------------------------------------------------------------------------------------------
inline void _Atom_libassert_CustomFailAction(libassert::assert_type type,
    libassert::ASSERTION fatal, const libassert::assertion_printer& printer);

/// Assertion in all modes.
/// ------------------------------------------------------------------------------------------------
#define ATOM_ASSERT(expr, ...) \
    ASSERT_INVOKE(expr, false, true, "ASSERT", debug_assertion, , __VA_ARGS__)

/// Assertion in Debug mode only.
/// ------------------------------------------------------------------------------------------------
#if ATOM_IS_CONFIG_DEBUG
#define ATOM_DEBUG_ASSERT(expr, ...) \
        ASSERT_INVOKE(expr, false, true, "DEBUG_ASSERT", debug_assertion, , __VA_ARGS__)

#else
#define ATOM_DEBUG_ASSERT(expr, ...) (void)0

#endif

/// 
/// ------------------------------------------------------------------------------------------------
#define ATOM_ASSERT_THROW(assertion, exception) \
    if (!(assertion)) throw exception

/// 
/// ------------------------------------------------------------------------------------------------
#define ATOM_DEBUG_ASSERT_THROW(assertion, exception) \
    ATOM_IF_DEBUG ATOM_ASSERT_THROW(assertion, exception)
