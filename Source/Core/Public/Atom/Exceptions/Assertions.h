#pragma once
#include "Atom/Exceptions/Exceptions.h"

namespace Atom
{
    using AssertExpr = const char*;

    /// --------------------------------------------------------------------------------------------
    /// Represents an assertion failure.
    /// --------------------------------------------------------------------------------------------
    class AssertErr: public Err
    {
    public:
        AssertErr(ErrMsg msg, AssertExpr assertExpr):
            Err(mov(msg)), assertExpr{ assertExpr } {}

    public:
        AssertExpr assertExpr;
    };
}

namespace Atom::Internal
{
    class AssertionFailure: Ex::Internal::Thrower
    {
    public:
        using Thrower = Ex::Internal::Thrower;

    public:
        AssertionFailure(AssertExpr assertExpr, ErrSource src = ErrSource::Current(),
            StackTrace stackTrace = StackTrace::current()):
            assertExpr{ assertExpr },
            Thrower(mov(src), mov(stackTrace))
        {}

        ~AssertionFailure()
        {
            if (!done)
            {
                *this << AssertErr("Assertion failed.", assertExpr);
            }
        }

    public:
        template <typename TEx>
        [[noreturn]]
        void operator<<(TEx&& ex)
            requires(RDerivedFrom<TEx, Err>)
        {
            done = true;

            // Thrower::operator<<(fwd(ex));
            throw 0;
        }

        template <typename TArg>
        [[noreturn]]
        void operator<<(TArg&& arg)
            requires(RNotDerivedFrom<TArg, Err>)
        {
            done = true;

            throw 0;
            // Thrower::operator<<AssertErr(fwd(arg), assertExpr);
        }

    public:
        bool done = false;
        AssertExpr assertExpr;
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
