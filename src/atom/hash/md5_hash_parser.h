#pragma once
#include "md5_hash.h"
#include "private/t1_hash_parser.h"

namespace atom
{
    using md5_hash_parser = _t1_hash_parser<md5_hash>;
}
