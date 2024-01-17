#pragma once
#include "private/t1_hash_parser.h"
#include "sha1_hash.h"

namespace atom
{
    using sha1_hash_parser = _t1_hash_parser<sha1_hash>;
}
