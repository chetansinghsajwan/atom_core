export module atom.core:strings.string_literal;

import :ranges;

namespace atom
{
    export using string_literal = range_literal<char>;
}
