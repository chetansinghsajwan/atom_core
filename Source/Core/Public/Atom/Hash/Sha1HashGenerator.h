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
        void Initialize()
        {
            Sha1Initialise(&m_context);
        }

        void Update(const void* data, uint32_t dataSize)
        {
            Sha1Update(&m_context, data, dataSize);
        }

        void Calculate(Sha1Hash& hash)
        {
            Sha1Finalise(&m_context, (SHA1_HASH*)hash.bytes.data());
        }

        Sha1Context m_context;
    };
}

namespace Atom
{
    using Sha1HashGenerator = Private::T1HashGenerator<Sha1Hash, Private::Sha1GeneratorImpl>;
}