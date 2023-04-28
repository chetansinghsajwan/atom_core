#pragma once
#include "Exceptions.decl.h"
#include "Atom/String/String.h"
#include "Atom/String/StringView.h"

namespace Atom
{
    inline Exception::Exception(String in_msg) noexcept:
        m_msg(MOVE(in_msg)) { }

    inline StringView Exception::GetMsg() const noexcept
    {
        return m_msg;
    }
}