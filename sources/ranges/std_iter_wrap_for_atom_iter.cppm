export module atom.core:ranges.std_iter_wrap_for_atom_iter;

import std;
import :core;
import :types;
import :contracts;
import :ranges.iter_requirements;

namespace atom
{
    export template <typename iter_t>
    class std_iter_wrap_for_atom_iter: public iter_t
    {};
}
