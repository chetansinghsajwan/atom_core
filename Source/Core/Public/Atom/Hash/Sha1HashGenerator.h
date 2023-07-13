#pragma once
extern "C"
{
#include "WjCryptLib_Sha1.h"
}

#include "Sha1Hash.h"
#include "Private/T1HashGenerator.h"

namespace Atom::Private
{
    class Sha1GeneratorImpl
    {
        pub fn Initialize()
        {
            Sha1Initialise(&_context);
        }

        pub fn Update(const void* data, uint32_t dataSize)
        {
            Sha1Update(&_context, data, dataSize);
        }

        pub fn Calculate(Sha1Hash& hash)
        {
            Sha1Finalise(&_context, (SHA1_HASH*)hash.bytes.Data());
        }

        pub Sha1Context _context;
    };
}

namespace Atom
{
    using Sha1HashGenerator = Private::T1HashGenerator<Sha1Hash, Private::Sha1GeneratorImpl>;
}