#pragma once
#include "atom/hash/sha1_hash_generator.h"
#include "atom/uuid/private/uuid_name_generator.h"
#include "atom/uuid/uuid.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// generates version 5 uuid using a name.
    /// --------------------------------------------------------------------------------------------
    using uuid5generator = _uuid_name_generator<sha1_hash, sha1_hash_generator, euuid_version::v5>;
}
