#pragma once
#include "Atom/Exceptions/Exceptions.h"
#include "Atom/Exceptions/Assertions.decl.h"

namespace Atom
{
    inline AssertionException::AssertionException(const char16* in_msg) noexcept:
        Exception(MOVE(in_msg)) { }
}

inline void _Atom_libassert_CustomFailAction(libassert::assert_type type,
    libassert::ASSERTION fatal, const libassert::assertion_printer& printer)
{
    using namespace Atom;
    using namespace libassert;

    ::std::string libassert_msg = printer(100);

    // TODO: Need ASCII conversion, casting ascii input to utf8 for now.
    // TODO: Resolve circular dependencies.
    // UTF8StringView utf8Msg{ (const char8*)libassert_msg.data(), libassert_msg.size() };
    // String msg = CharEncodingConverter<UTF8CharEncoding, CharEncoding>()
    //     .Convert(utf8Msg.Iterator());

    // throw AssertionException{ msg };

    throw AssertionException(u"Not implemented yet.");
}