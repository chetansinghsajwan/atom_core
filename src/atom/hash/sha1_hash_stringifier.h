#pragma once
#include "private/t1_hash_stringifier.h"
#include "sha1_hash.h"

namespace atom
{
    using sha1_hash_stringifier = _t1_hash_stringifier<sha1_hash>;
}
