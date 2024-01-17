#pragma once
#include "Private/T1HashParser.h"
#include "Sha1Hash.h"

namespace Atom
{
    using Sha1HashParser = Private::T1HashParser<Sha1Hash>;
}
