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
    public:
        auto Initialize()
        {
            Md5Initialise(&_context);
        }

        auto Update(ConstMemPtr<void> data, uint32_t dataSize)
        {
            Md5Update(&_context, data.raw(), dataSize);
        }

        auto Calculate(Md5Hash& hash)
        {
            Md5Finalise(&_context, reinterpret_cast<MD5_HASH*>(hash.bytes.mutMem().raw()));
        }

    public:
        Md5Context _context;
    };
}

namespace Atom
{
    using Md5HashGenerator = Private::T1HashGenerator<Md5Hash, Private::Md5GeneratorImpl>;
}
