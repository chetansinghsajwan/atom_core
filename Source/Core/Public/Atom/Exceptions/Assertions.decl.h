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
        AssertException(Str msg, StrView assertExpr);

    public:
        StrView assertExpr;
    };
}

namespace Atom::Internal
{
    class AssertionFailure: Ex::Internal::Thrower
    {
    public:
        using Thrower = Ex::Internal::Thrower;

    public:
        AssertionFailure(StrView assertExpr, ExceptionSource src = ExceptionSource::Current(),
            StackTrace stackTrace = StackTrace::current())
            : assertExpr{ assertExpr }
            , Thrower(mov(src), mov(stackTrace))
        {}

        ~AssertionFailure()
        {
            if (!done)
            {
                *this << AssertException("Assertion failed.", assertExpr);
            }
        }

    public:
        template <typename TEx>
        [[noreturn]]
        void
        operator<<(TEx&& ex)
            requires(RDerivedFrom<TEx, Exception>)
        {
            done = true;

            // Thrower::operator<<(fwd(ex));
            throw 0;
        }

        template <typename TArg>
        [[noreturn]]
        void
        operator<<(TArg&& arg)
            requires(RNotDerivedFrom<TArg, Exception>)
        {
            done = true;

            throw 0;
            // Thrower::operator<<AssertException(fwd(arg), assertExpr);
        }

    public:
        bool done = false;
        StrView assertExpr;
    };
}

/// ------------------------------------------------------------------------------------------------
/// Assertion in all modes.
/// ------------------------------------------------------------------------------------------------
#define ATOM_ASSERT(assertion)                                                                     \
    if (!(assertion))                                                                              \
    ::Atom::Internal::AssertionFailure(#assertion)

/// ------------------------------------------------------------------------------------------------
/// Assertion in Debug mode only.
/// ------------------------------------------------------------------------------------------------
#if ATOM_IS_CONFIG_DEBUG
#    define ATOM_DEBUG_ASSERT(assertion) ATOM_ASSERT(assertion)
#else
#    define ATOM_DEBUG_ASSERT(assertion) ATOM_ASSERT(true)
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

[[noreturn]]
void myTerminate(auto& msg)
{
    std::cout << msg << std::endl;
    std::terminate();
}
