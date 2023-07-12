#pragma once
#include <random>

#include "Atom/Memory.h"
#include "Atom/Random.h"

#include "Atom/Uuid/Uuid.h"

namespace Atom
{
    template <tname TUniformRandomNumberGenerator>
    class BasicUuidRandomGenerator
    {
    public:
        explicit BasicUuidRandomGenerator(uint32 seed = 0) noex:
            m_generator{ seed } { }

    public:
        Uuid Generate() noex
        {
            Uuid uuid;
            for (usize i = 0; i < 16; i += 4)
            {
                *(uint32*)(uuid.bytes.Data() + i) = m_distribution(m_generator);
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
        UniformIntDistribution<uint32> m_distribution;
        TUniformRandomNumberGenerator m_generator;
    };

    /// --------------------------------------------------------------------------------------------
    /// Generates version 4 random Uuid.
    /// --------------------------------------------------------------------------------------------
    using Uuid4Generator = BasicUuidRandomGenerator<MT19937>;
}