#pragma once
#include "Exceptions.decl.h"
#include "Atom/Str/Str.h"
#include "Atom/Str/StrView.h"
#include "Atom/Str/AsciiStrView.h"

namespace Atom
{
    constexpr ExceptionSource ExceptionSource::Current(TImpl src)
    {
        return ExceptionSource
        {
            .line = src.line(),
            .column = src.column(),
            // TODO: Fix this usage.
            .fileName = Range(src.file_name(), usize(0)),
            .funcName = Range(src.function_name(), usize(0))
        };
    }

    inline Exception::Exception(Str msg):
        msg(mov(msg)) { }

    inline Exception::~Exception() { }

    inline IndexOutOfRangeException::IndexOutOfRangeException(
        Str msg, usize index, usize begin, usize end):
        OutOfRangeException(mov(msg)), index{ index }, begin{ begin }, end{ end } { }

    inline IndexOutOfRangeException::IndexOutOfRangeException(
        usize index, usize begin, usize end):
        OutOfRangeException(),
        index{ index }, begin{ begin }, end{ end } { }
}

namespace Atom::Ex::Internal
{
    inline Thrower::Thrower(ExceptionSource src, StackTrace stackTrace):
        _src{ mov(src) }, _stackTrace{ mov(stackTrace) } { }

    inline auto Thrower::RecordStack(StackTrace stackTrace) -> Thrower&
    {
        this->_stackTrace = mov(stackTrace);
        return *this;
    }

    inline auto Thrower::RecordSource(ExceptionSource src) -> Thrower&
    {
        this->_src = mov(src);
        return *this;
    }

    template <typename TEx>
    inline auto Thrower::operator<<(TEx&& ex)
        requires(RDerivedFrom<TEx, Exception>)
    {
        ex.src = mov(_src);
        ex.stackTrace = mov(_stackTrace);

        throw ex;
    }
}
