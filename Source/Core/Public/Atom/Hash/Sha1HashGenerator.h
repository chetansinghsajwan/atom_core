#pragma once
extern "C"
{
#include "WjCryptLib_Sha1.h"
}

#include "Sha1Hash.h"
#include "Private/T1HashGenerator.h"

namespace Atom::Private
{
    struct Sha1GeneratorImpl
    {
        fn Initialize()
        {
            Sha1Initialise(&_context);
        }

        fn Update(const void* data, uint32_t dataSize)
        {
            Sha1Update(&_context, data, dataSize);
        }

        fn Calculate(Sha1Hash& hash)
        {
            Sha1Finalise(&_context, (SHA1_HASH*)hash.bytes.Data());
        }

        Sha1Context _context;
    };
}

namespace Atom
{
    using Sha1HashGenerator = Private::T1HashGenerator<Sha1Hash, Private::Sha1GeneratorImpl>;
}