#include "BasicStaticString.h"
#include "CharEncoding.h"

namespace Atom
{
    template <usize size>
    using StaticString = BasicStaticString<CharEncoding, size>;
}