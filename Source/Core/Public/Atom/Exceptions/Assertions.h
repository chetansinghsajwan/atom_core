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
            // String msg{ FORWARD(args)... };
            // throw TException((const char*)msg.data());
        }
    }

    template <typename TException = AssertionException>
    void DEBUG_ASSERT(bool assertion, auto&&... args)
    {
        ATOM_DEBUG_IF
        {
            ASSERT<TException>(assertion, FORWARD(args)...);
        }
    }
}

#define STATIC_ASSERT_MSG(assertion, msg) static_assert(assertion, msg)
#define STATIC_ASSERT(assertion) static_assert(assertion)
#define STATIC_THROW(msg) static_assert(false, msg)

// #define ATOM_DEBUG_EXPECTS_MSG(assertion, msg) DEBUG_ASSERT<AssertionException>(assertion, \
//     ::Atom::StringASCII(ATOM_FUNCTION) + TEXT(": [DEBUG] Expects {" #assertion "}. ") msg)

#define ATOM_DEBUG_EXPECTS_MSG(assertion, msg) \
    DEBUG_ASSERT<AssertionException>(assertion, \
    TEXT(": [DEBUG] Expects {") TEXT(#assertion) TEXT("}. ") msg)

#define ATOM_DEBUG_EXPECTS(assertion) ATOM_DEBUG_EXPECTS_MSG(assertion, TEXT(""))