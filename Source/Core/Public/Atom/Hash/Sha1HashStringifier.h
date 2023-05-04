#pragma once
#include "Sha1Hash.h"
#include "Private/T1HashStringifier.h"

namespace Atom
{
    using Sha1HashStringifier = Private::T1HashStringifier<Sha1Hash>;
}