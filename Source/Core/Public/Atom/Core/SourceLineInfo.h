#pragma once
#include "Atom/Core/Primitives.h"
#include <source_location>

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Info about the source code line.
    /// --------------------------------------------------------------------------------------------
    class SourceLineInfo
    {
    public:
        /// --------------------------------------------------------------------------------------------
        /// Get the source line info of the calling site.
        ///
        /// # Note
        /// - This default argument is used to capture the source line info of the calling site.
        ///   Don't pass any argument for this.
        /// --------------------------------------------------------------------------------------------
#if defined(ATOM_COMP_CLANG) && !defined(__cpp_lib_source_location)
        static consteval auto current(
            const char *fileName = __builtin_FILE(),
            const char *funcName = __builtin_FUNCTION(),
            u32 line = __builtin_LINE(),
            u32 column = __builtin_COLUMN()) -> SourceLineInfo
        {
            return SourceLineInfo
            {
                .line = line,
                .column = column,
                .funcName = funcName,
                .fileName = fileName
            };
        }
#else
        static consteval auto current(std::source_location src = 
            std::source_location::current()) -> SourceLineInfo
        {
            return SourceLineInfo
            {
                .line = src.line(),
                .column = src.column(),
                .funcName = src.function_name(),
                .fileName = src.file_name()
            };
        }
#endif

    public:
        u32 line;
        u32 column;
        const char* funcName;
        const char* fileName;
    };
}
