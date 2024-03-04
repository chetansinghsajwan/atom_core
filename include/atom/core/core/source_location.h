#pragma once
#include "atom/core/_std.h"
#include "atom/core/core.h"
#include "atom/core/preprocessors.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// info about the source code line.
    /// --------------------------------------------------------------------------------------------
    class source_line_info
    {
    public:
        /// --------------------------------------------------------------------------------------------
        /// get the source line info of the calling site.
        ///
        /// # note
        /// - this_type default argument is used to capture the source line info of the calling site.
        ///   do not pass any argument for this.
        /// --------------------------------------------------------------------------------------------
#if defined(ATOM_COMPILER_CLANG) && !defined(__cpp_lib_source_location)
        static consteval auto current(const char* file_name = __builtin_FILE(),
            const char* func_name = __builtin_FUNCTION(), u32 line = __builtin_LINE(),
            u32 column = __builtin_COLUMN()) -> source_line_info
        {
            return source_line_info{
                .line = line, .column = column, .func_name = func_name, .file_name = file_name
            };
        }
#else
        static consteval auto current(std::source_location src = std::source_location::current())
            -> source_line_info
        {
            return source_line_info{ .line = src.line(),
                .column = src.column(),
                .func_name = src.function_name(),
                .file_name = src.file_name() };
        }
#endif

    public:
        u32 line;
        u32 column;
        std::string_view func_name;
        std::string_view file_name;
    };
}
