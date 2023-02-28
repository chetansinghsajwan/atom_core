#pragma once
#include "Atom/Core.h"
#include "Atom/Exceptions/Exceptions.h"

namespace Atom
{
    template <typename TException = AssertionException>
    void ASSERT(bool assertion, auto&&... args)
    {
        if (!assertion)
        {
            throw TException(FORWARD(args)...);
        }
    }

    template <typename TException = AssertionException>
    void DEBUG_ASSERT(bool assertion, auto&&... args)
    {
    #ifdef ATOM_CONFIG_DEBUG
        ASSERT<TException>(assertion, FORWARD(args)...);
    #endif
    }
}

#define STATIC_ASSERT_MSG(assertion, msg) static_assert(assertion, msg)
#define STATIC_ASSERT(assertion) static_assert(assertion)
#define STATIC_THROW(msg) static_assert(false, msg)

#define ATOM_DEBUG_EXPECTS_MSG(assertion, msg) DEBUG_ASSERT<AssertionException>(assertion, \
    String(ATOM_FUNCTION) + ": [DEBUG] Expects {" #assertion "}. " msg)

#define ATOM_DEBUG_EXPECTS(assertion) ATOM_DEBUG_EXPECTS_MSG(assertion, "")