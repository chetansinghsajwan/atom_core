export module atom.core;
import :std;
import :fmt;
import :wjcrypt;
export import :core;
export import :build_config;
export import :contracts_decl;
export import :requirements;
export import :time;
export import :tti;
export import :random;
export import :ptr;
export import :mem_ptr;
export import :array_iter;
export import :range;
export import :memory.legacy_mem_allocator;
export import :memory.default_mem_allocator;

export
{
#include "atom/core_all.h"
}
