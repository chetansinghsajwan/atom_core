#pragma once
#include "Atom/Exceptions/Exceptions.decl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Represents an assertion failure.
    /// --------------------------------------------------------------------------------------------
    class AssertException: public Exception
    {
    public:
        AssertException(Str msg, StrView assertExpr) noex;

    public:
        StrView assertExpr;
    };
}

namespace Atom::Internal
{
    struct AssertionFailure: Ex::Internal::Thrower
    {
        using Thrower = Ex::Internal::Thrower;

        AssertionFailure(StrView assertExpr, 
            ExceptionSource src = ExceptionSource::Current(),
            StackTrace stackTrace = StackTrace::current()):
            assertExpr{ assertExpr }, Thrower(MOVE(src), MOVE(stackTrace)) { }

        ~AssertionFailure()
        {
            if (!done)
            {
                *this << AssertException(TEXT("Assertion failed."), assertExpr);
            }
        }

        template <tname TEx>
        requires RDerivedFrom<TEx, Exception>
        ATOM_ATTR_NORETURN
        void operator << (TEx&& ex)
        {
            done = true;

            Thrower::operator << (fwd(ex));
        }

        template <tname TArg>
        requires RNotDerivedFrom<TArg, Exception>
        ATOM_ATTR_NORETURN
        void operator << (TArg&& arg)
        {
            done = true;

            throw 0;
            // Thrower::operator<< AssertException(fwd(arg), assertExpr);
        }

        bool done = false;
        StrView assertExpr;
    };
}

/// ------------------------------------------------------------------------------------------------
/// Assertion in all modes.
/// ------------------------------------------------------------------------------------------------
#define ATOM_ASSERT(assertion) \
    if (!(assertion)) ::Atom::Internal::AssertionFailure(TEXT(#assertion))

/// ------------------------------------------------------------------------------------------------
/// Assertion in Debug mode only.
/// ------------------------------------------------------------------------------------------------
#if ATOM_IS_CONFIG_DEBUG
    #define ATOM_DEBUG_ASSERT(assertion) ATOM_ASSERT(assertion)
#else
    #define ATOM_DEBUG_ASSERT(assertion) ATOM_ASSERT(true)
#endif

/// ------------------------------------------------------------------------------------------------
/// Represents b{Pre Condition Assertions}.
/// ------------------------------------------------------------------------------------------------
#define ATOM_EXPECTS(assertion) ATOM_ASSERT(assertion)

/// ------------------------------------------------------------------------------------------------
/// Represents b{Pre Condition Assertions}.
/// ------------------------------------------------------------------------------------------------
#define ATOM_DEBUG_EXPECTS(assertion) ATOM_DEBUG_ASSERT(assertion)

/// ------------------------------------------------------------------------------------------------
/// Represents b{Post Condition Assertions}.
/// ------------------------------------------------------------------------------------------------
#define ATOM_ENSURES(assertion) ATOM_ASSERT(assertion)

/// ------------------------------------------------------------------------------------------------
/// Represents b{Post Condition Assertions}.
/// ------------------------------------------------------------------------------------------------
#define ATOM_DEBUG_ENSURES(assertion) ATOM_DEBUG_ASSERT(assertion)
