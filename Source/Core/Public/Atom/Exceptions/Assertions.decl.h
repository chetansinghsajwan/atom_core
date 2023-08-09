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
        ctor AssertException(Str msg, StrView assertExpr);

    public:
        StrView assertExpr;
    };
}

namespace Atom::Internal
{
    class AssertionFailure: Ex::Internal::Thrower
    {
        pub using Thrower = Ex::Internal::Thrower;

        pub ctor AssertionFailure(StrView assertExpr, 
            ExceptionSource src = ExceptionSource::Current(),
            StackTrace stackTrace = StackTrace::current()):
            assertExpr{ assertExpr }, Thrower(mov(src), mov(stackTrace)) { }

        pub dtor AssertionFailure()
        {
            if (!done)
            {
                self << AssertException("Assertion failed.", assertExpr);
            }
        }

        pub template <typename TEx>
        requires RDerivedFrom<TEx, Exception>
        noret void op<<(TEx&& ex)
        {
            done = true;

            // Thrower::op<<(fwd(ex));
            throw 0;
        }

        pub template <typename TArg>
        requires RNotDerivedFrom<TArg, Exception>
        noret void op<<(TArg&& arg)
        {
            done = true;

            throw 0;
            // Thrower::op<<AssertException(fwd(arg), assertExpr);
        }

        pub bool done = false;
        pub StrView assertExpr;
    };
}

/// ------------------------------------------------------------------------------------------------
/// Assertion in all modes.
/// ------------------------------------------------------------------------------------------------
#define ATOM_ASSERT(assertion) \
    if (!(assertion)) ::Atom::Internal::AssertionFailure(#assertion)

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

noret void myTerminate(auto& msg)
{
    std::cout << msg << std::endl;
    std::terminate();
}
