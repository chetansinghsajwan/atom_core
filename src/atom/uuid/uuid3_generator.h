#pragma once
#include "atom/hash/md5_hash_generator.h"
#include "atom/uuid/private/uuid_name_generator.h"
#include "atom/uuid/uuid.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// generates version 3 uuid using a name.
    /// --------------------------------------------------------------------------------------------
    using uuid3generator = _uuid_name_generator<md5_hash, md5_hash_generator, euuid_version::v3>;
}
