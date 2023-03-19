#pragma once
#include "Atom/Core.h"
#include "Atom/Exceptions/Exceptions.h"

#define ATOM_ASSERT(assertion, exception) if (!(assertion)) throw exception
#define ATOM_DEBUG_ASSERT(assertion, exception) ATOM_DEBUG_IF if (!(assertion)) throw exception

#define ATOM_DEBUG_EXPECTS_MSG(assertion, msg) \
    ATOM_DEBUG_ASSERT(assertion, AssertionException( \
    Atom::String{ (const Char*)ATOM_FUNCTION } + \
    TEXT(": [DEBUG] Expects {") TEXT(#assertion) TEXT("}. ") msg))

#define ATOM_DEBUG_EXPECTS(assertion) ATOM_DEBUG_EXPECTS_MSG(assertion, TEXT(""))