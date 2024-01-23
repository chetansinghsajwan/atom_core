export module atom.core:string_literal;
import :core;
import :range.range_literal;

namespace atom
{
    export using string_literal = range_literal<uchar>;
}