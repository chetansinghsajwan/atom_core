#pragma once
#include "Atom/Memory.h"
#include "Atom/Random.h"

#include "Atom/Uuid/Uuid.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// # Template Parameters
    /// 
    /// - `TGenerator`: Uniform random number generator.
    /// --------------------------------------------------------------------------------------------
    template <typename TGenerator>
    class BasicUuidRandomGenerator
    {
    public:
        explicit BasicUuidRandomGenerator(u32 seed = 0)
            : _generator{ seed } {}

    public:
        auto generate() -> Uuid
        {
            Uuid uuid;
            for (usize i = 0; i < 16; i += 4)
            {
                u32& out = (uuid.bytes.mutMem() + i).as<u32>().getMut();
                out = _distribution(_generator);
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
        UniformIntDistribution<_u32> _distribution;
        TGenerator _generator;
    };

    /// --------------------------------------------------------------------------------------------
    /// Generates version 4 random Uuid.
    /// --------------------------------------------------------------------------------------------
    using Uuid4Generator = BasicUuidRandomGenerator<MT19937>;
}
