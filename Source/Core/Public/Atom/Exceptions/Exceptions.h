#pragma once
#include "Exceptions.decl.h"
#include "Atom/String/String.h"
#include "Atom/String/StringView.h"
#include "Atom/String/ASCIIStringView.h"

namespace Atom
{
    constexpr ExceptionSource ExceptionSource::Current(TImpl src) noexcept
    {
        return ExceptionSource
        {
            .line = src.line(),
            .column = src.column(),
            .fileName = src.file_name(),
            .funcName = src.function_name()
        };
    }

    inline Exception::Exception(String msg) noexcept:
        msg(MOVE(msg)) { }

    inline Exception::~Exception() { }
}

namespace Atom::Ex::Internal
{
    inline Thrower::Thrower(ExceptionSource src) noexcept:
        src{ src }, stackTrace{ } { }

    inline Thrower& Thrower::RecordStack(StackTrace stackTrace)
    {
        this->stackTrace = MOVE(stackTrace);
        return *this;
    }

    inline Thrower& Thrower::RecordSource(ExceptionSource src)
    {
        this->src = MOVE(src);
        return *this;
    }

    template <typename TEx>
    requires RDerivedFrom<TEx, Exception>
    inline void Thrower::operator << (TEx&& ex)
    {
        ex.src = MOVE(src);
        ex.stackTrace = MOVE(stackTrace);

        throw ex;
    }
}
