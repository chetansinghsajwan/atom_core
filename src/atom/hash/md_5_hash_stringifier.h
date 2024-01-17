#pragma once
#include "Md5Hash.h"
#include "Private/T1HashStringifier.h"

namespace Atom
{
    using Md5HashStringifier = Private::T1HashStringifier<Md5Hash>;
}
