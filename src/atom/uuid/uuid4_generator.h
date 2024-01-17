#pragma once
#include "atom/memory.h"
#include "atom/random.h"

#include "atom/uuid/uuid.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// # template parameters
    ///
    /// - `tgenerator`: uniform random number generator.
    /// --------------------------------------------------------------------------------------------
    template <typename tgenerator>
    class basic_uuid_random_generator
    {
    public:
        explicit basic_uuid_random_generator(u32 seed = 0)
            : _generator{ seed }
        {}

    public:
        auto generate() -> uuid
        {
            uuid uuid;
            for (usize i = 0; i < 16; i += 4)
            {
                u32& out = (uuid.bytes.mut_mem() + i).as<u32>().get_mut();
                out = _distribution(_generator);
            }

            // variant must be 10xxxxxx
            uuid.bytes[8] &= 0xbf;
            uuid.bytes[8] |= 0x80;

            // version must be 0100xxxx
            uuid.bytes[6] &= 0x4f;
            uuid.bytes[6] |= 0x40;

            return uuid;
        }

    private:
        uniform_int_distribution<_u32> _distribution;
        tgenerator _generator;
    };

    /// --------------------------------------------------------------------------------------------
    /// generates version 4 random uuid.
    /// --------------------------------------------------------------------------------------------
    using uuid4generator = basic_uuid_random_generator<mt19937>;
}
