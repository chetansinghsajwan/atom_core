#pragma once
#include "Atom/Hash.h"

#include "Atom/Uuid/Uuid.h"
#include "Atom/Uuid/Private/UuidNameGenerator.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Generates version 5 Uuid using a name.
    /// --------------------------------------------------------------------------------------------
    using Uuid5Generator = Private::UuidNameGenerator<
        Sha1Hash, Sha1HashGenerator, EUuidVersion::V5>;
}