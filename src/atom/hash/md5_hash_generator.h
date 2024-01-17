#pragma once
extern "C"
{
#include "WjCryptLib_Md5.h"
}

#include "md5_hash.h"
#include "private/t1_hash_generator.h"

namespace atom
{
    class _md5_generator_impl
    {
    public:
        auto initialize()
        {
            Md5Initialise(&_context);
        }

        auto update(mem_ptr<void> data, uint32_t data_size)
        {
            Md5Update(&_context, data.unwrap(), data_size);
        }

        auto calculate(md5_hash& hash)
        {
            Md5Finalise(&_context, hash.bytes.mut_mem().as<MD5_HASH>().unwrap());
        }

    public:
        Md5Context _context;
    };

    using md5_hash_generator = _t1_hash_generator<md5_hash, _md5_generator_impl>;
}
