#pragma once
extern "C"
{
#include "WjCryptLib_Md5.h"
}

#include "Md5Hash.h"
#include "Private/T1HashGenerator.h"

namespace Atom::Private
{
    class Md5GeneratorImpl
    {
        pub fn Initialize()
        {
            Md5Initialise(&_context);
        }

        pub fn Update(const void* data, uint32_t dataSize)
        {
            Md5Update(&_context, data, dataSize);
        }

        pub fn Calculate(Md5Hash& hash)
        {
            Md5Finalise(&_context, (MD5_HASH*)hash.bytes.Data());
        }

        pub Md5Context _context;
    };
}

namespace Atom
{
    using Md5HashGenerator = Private::T1HashGenerator<Md5Hash, Private::Md5GeneratorImpl>;
}