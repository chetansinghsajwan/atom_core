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
    private:
        using _TImpl = std::source_location;

    public:
        /// --------------------------------------------------------------------------------------------
        /// Get the source line info of the calling site.
        /// 
        /// # Note
        /// - This default argument is used to capture the source line info of the calling site.
        ///   Don't pass any argument for this.
        /// --------------------------------------------------------------------------------------------
        static constexpr SourceLineInfo current(_TImpl src = _TImpl::current())
        {
            return SourceLineInfo {
                .line = src.line(),
                .column = src.column(),
                .fileName = src.file_name(),
                .funcName = src.function_name()
            };
        }

    public:
        u32 line;
        u32 column;
        const char* fileName;
        const char* funcName;
    };
}