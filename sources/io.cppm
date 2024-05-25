module;
#include <cstdio>

export module atom.core:io;

import :filesystem;

namespace atom::io
{
    export filesystem::file stdin{ ::stdin, filesystem::file::open_flags::read };
    export filesystem::file stdout{ ::stdout, filesystem::file::open_flags::write };
    export filesystem::file stderr{ ::stderr, filesystem::file::open_flags::write };
}
