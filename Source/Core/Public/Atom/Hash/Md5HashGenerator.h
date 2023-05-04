#pragma once
extern "C"
{
#include "WjCryptLib_Md5.h"
}

#include "Md5Hash.h"
#include "Private/T1HashGenerator.h"

namespace Atom::Private
{
    struct Md5GeneratorImpl
    {
        void Initialize()
        {
            Md5Initialise(&m_context);
        }

        void Update(const void* data, uint32_t dataSize)
        {
            Md5Update(&m_context, data, dataSize);
        }

        void Calculate(Md5Hash& hash)
        {
            Md5Finalise(&m_context, (MD5_HASH*)hash.bytes.data());
        }

        Md5Context m_context;
    };
}

namespace Atom
{
    using Md5HashGenerator = Private::T1HashGenerator<Md5Hash, Private::Md5GeneratorImpl>;
}