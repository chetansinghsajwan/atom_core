#pragma once
#include "Md5Hash.h"
#include "Private/T1HashParser.h"

namespace Atom
{
    using Md5HashParser = Private::T1HashParser<Md5Hash>;
}