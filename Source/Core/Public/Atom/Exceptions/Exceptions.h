#pragma once
#include "Exceptions.decl.h"
#include "Atom/String/String.h"
#include "Atom/String/StringView.h"
#include "Atom/String/ASCIIStringView.h"

namespace Atom
{
    inline Exception::Exception(String msg) noexcept:
        msg(MOVE(msg)) { }

    inline Exception::~Exception() { }
}