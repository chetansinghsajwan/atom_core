#pragma once
#include "Private/T1HashStringifier.h"
#include "Sha1Hash.h"

namespace Atom
{
    using Sha1HashStringifier = Private::T1HashStringifier<Sha1Hash>;
}
