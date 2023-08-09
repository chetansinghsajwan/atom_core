#pragma once
#include "Atom/Memory.h"
#include "Atom/Random.h"

#include "Atom/Uuid/Uuid.h"

namespace Atom
{
    template <typename TUniformRandomNumberGenerator>
    class BasicUuidRandomGenerator
    {
    public:
        explicit ctor BasicUuidRandomGenerator(u32 seed = 0) noex:
            _generator{ seed } { }

    public:
        fn generate() noex -> Uuid
        {
            Uuid uuid;
            for (usize i = 0; i < 16; i += 4)
            {
                *(u32*)(uuid.bytes.data() + i) = _distribution(_generator);
            }

            // Variant must be 10xxxxxx
            uuid.bytes[8] &= 0xBF;
            uuid.bytes[8] |= 0x80;

            // Version must be 0100xxxx
            uuid.bytes[6] &= 0x4F;
            uuid.bytes[6] |= 0x40;

            return uuid;
        }

    private:
        UniformIntDistribution<u32> _distribution;
        TUniformRandomNumberGenerator _generator;
    };

    /// --------------------------------------------------------------------------------------------
    /// Generates version 4 random Uuid.
    /// --------------------------------------------------------------------------------------------
    using Uuid4Generator = BasicUuidRandomGenerator<MT19937>;
}