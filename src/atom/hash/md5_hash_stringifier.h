#pragma once
#include "md5_hash.h"
#include "private/t1_hash_stringifier.h"

namespace atom
{
    using md5_hash_stringifier = _t1_hash_stringifier<md5_hash>;
}
