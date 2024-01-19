#pragma once
#include "sha1_hash.h"
#include "private/t1_hash_generator.h"

namespace atom
{
    class _sha1_generator_impl
    {
    public:
        auto initialize()
        {
            Sha1Initialise(&_context);
        }

        auto update(mem_ptr<void> data, std::uint32_t data_size)
        {
            Sha1Update(&_context, data.unwrap(), data_size);
        }

        auto calculate(sha1_hash& hash)
        {
            Sha1Finalise(&_context, hash.bytes.mut_mem().as<SHA1_HASH>().unwrap());
        }

    public:
        Sha1Context _context;
    };

    using sha1_hash_generator = _t1_hash_generator<sha1_hash, _sha1_generator_impl>;
}
