#pragma once
#include "Atom/Exceptions/Assertions.decl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Contains basic memory functions.
    /// --------------------------------------------------------------------------------------------
    struct MemHelper
    {
        /// ----------------------------------------------------------------------------------------
        /// Sets each mem unit of mem block [dest, dest + count] with value {val}.
        /// 
        /// @PARAM dest: Memory location to write to.
        /// @PARAM count: Count of mem units to write.
        /// @PARAM val: Value to write.
        /// 
        /// @EXCEPTS[debug] dest != nullptr.
        /// @EXCEPTS[debug] count > 0.
        /// ----------------------------------------------------------------------------------------
        constexpr void Set(void* dest, usize count, byte val) const noexcept
        {
            ATOM_DEBUG_EXPECTS(dest != nullptr);
            ATOM_DEBUG_EXPECTS(count > 0);

            std::memset(dest, val, count);
        }

        /// ----------------------------------------------------------------------------------------
        /// Same as {Set()}, but the call will not be optimized away.
        /// ----------------------------------------------------------------------------------------
        constexpr void SetExplicit(void* dest, usize count, byte val) const noexcept
        {
        #pragma optimize("", off)
            Set(dest, count, val);
        #pragma optimize("", on)
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies {count} count mem units from mem {src} to mem {dest}.
        /// 
        /// @PARAM dest: Memory location to read from.
        /// @PARAM dest: Memory location to write to.
        /// @PARAM count: Count of mem units to write.
        /// 
        /// @EXCEPTS[debug] src != nullptr.
        /// @EXCEPTS[debug] dest != nullptr.
        /// @EXCEPTS[debug] count > 0.
        /// @EXCEPTS[debug] src - dest > count: Mem block to read from overlaps with mem block 
        ///     to write to.
        /// 
        /// @NOTE: If the mem block to read from and mem block to write to overlaps, 
        ///     the behaviour is undefined.
        /// 
        /// @SEE CopySafe().
        /// ----------------------------------------------------------------------------------------
        constexpr void Copy(const void* src, usize count, void* dest) const noexcept
        {
            ATOM_DEBUG_EXPECTS(src != nullptr);
            ATOM_DEBUG_EXPECTS(dest != nullptr);
            ATOM_DEBUG_EXPECTS(count > 0);

            // Check memory overlap.
            ATOM_DEBUG_EXPECTS((byte*)src - (byte*)dest > count);

            std::memcpy(dest, src, count);
        }

        /// ----------------------------------------------------------------------------------------
        /// Same as {Copy()}, but allows overlapping of mem block to read from and mem block to 
        /// write to.
        /// ----------------------------------------------------------------------------------------
        constexpr void CopySafe(const void* src, usize count, void* dest) const noexcept
        {
            ATOM_DEBUG_EXPECTS(src != nullptr);
            ATOM_DEBUG_EXPECTS(dest != nullptr);
            ATOM_DEBUG_EXPECTS(count > 0);

            std::memmove(dest, src, count);
        }
    };
}