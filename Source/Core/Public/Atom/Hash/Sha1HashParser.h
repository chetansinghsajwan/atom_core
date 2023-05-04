#pragma once
#include "Sha1Hash.h"
#include "Private/T1HashParser.h"

namespace Atom
{
    using Sha1HashParser = Private::T1HashParser<Sha1Hash>;
}