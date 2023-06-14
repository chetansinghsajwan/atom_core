#pragma once
#include "Atom/Exceptions/Assertions.decl.h"

#include <algorithm>

namespace Atom
{
    using MemUnit = byte;
    using MemPtr = byte*;

    /// --------------------------------------------------------------------------------------------
    /// Contains basic memory utility functions.
    /// --------------------------------------------------------------------------------------------
    struct MemHelper
    {
        /// ----------------------------------------------------------------------------------------
        /// Sets each mem unit of mem block {mem} with value {val}.
        /// 
        /// @PARAM mem: Mem block to write to.
        /// @PARAM val: Value to write.
        /// ----------------------------------------------------------------------------------------
        constexpr void Fill(MemPtr mem, usize count, MemUnit val) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(count > 0);

            _Fill(mem, count, val);
        }

        /// ----------------------------------------------------------------------------------------
        /// Same as {Fill(...)}, but the call will not be optimized away.
        /// ----------------------------------------------------------------------------------------
        #pragma optimize("", off)
        constexpr void FillExplicit(MemPtr mem, usize count, MemUnit val) const noexcept
        {
            Fill(mem, count, val);
        }
        #pragma optimize("", on)

        /// ----------------------------------------------------------------------------------------
        /// Copies {count} count mem units from mem {src} to mem {dest} using fwd iteration.
        /// 
        /// @PARAM src: Mem block to copy from.
        /// @PARAM dest: Mem location to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr void FwdCopy(const MemPtr src, usize count, MemPtr dest) const noexcept
        {
            ATOM_DEBUG_EXPECTS(src != nullptr);
            ATOM_DEBUG_EXPECTS(count > 0);
            ATOM_DEBUG_EXPECTS(dest != nullptr);

            ATOM_DEBUG_EXPECTS(dest < src) << ATOM_TEXT(
                "Src mem block overlaps with dest mem block.");

            _FwdCopy(src, count, dest);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies {count} count mem units from mem {src} to mem {dest} using bwd iteration.
        /// 
        /// @PARAM src: Mem block to copy from.
        /// @PARAM dest: Mem location to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr void BwdCopy(const MemPtr src, usize count, MemPtr dest) const noexcept
        {
            ATOM_DEBUG_EXPECTS(src != nullptr);
            ATOM_DEBUG_EXPECTS(count > 0);
            ATOM_DEBUG_EXPECTS(dest != nullptr);

            ATOM_DEBUG_EXPECTS(dest > (src + count)) << ATOM_TEXT(
                "Src mem block overlaps with dest mem block.");

            _BwdCopy(src, count, dest);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies {count} count mem units from mem {mem} to mem {mem + outset} 
        /// using fwd iteration.
        /// 
        /// @PARAM mem: Mem block to copy from.
        /// @PARAM outset: Count of mem units outset from {mem}.
        /// ----------------------------------------------------------------------------------------
        constexpr void CopyFwd(const MemPtr mem, usize count, usize outset) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(count > 0);
            ATOM_DEBUG_EXPECTS(outset > 0);

            _BwdCopy(mem, count, mem + outset);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies {count} count mem units from mem {mem} to mem {mem - inset} 
        /// using bwd iteration.
        /// 
        /// @PARAM mem: Mem block to copy from.
        /// @PARAM inset: Count of mem units inset from {mem}.
        /// ----------------------------------------------------------------------------------------
        constexpr void CopyBwd(const MemPtr mem, usize count, usize inset) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(count > 0);
            ATOM_DEBUG_EXPECTS(inset > 0);

            _FwdCopy(mem, count, mem - inset);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies {count} count mem units from mem {mem} to mem {mem + offset} 
        /// using appropriate iteration. This method is safe even if src and dest overlaps.
        /// 
        /// @PARAM mem: Mem block to copy from.
        /// @PARAM offset: Count of mem units offset from {mem}.
        /// ----------------------------------------------------------------------------------------
        constexpr void CopyBy(const MemPtr mem, usize count, isize offset) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(count > 0);
            ATOM_DEBUG_EXPECTS(offset != 0);

            if (offset > 0)
            {
                _BwdCopy(mem, count, mem + offset);
            }
            else
            {
                _FwdCopy(mem, count, mem + offset);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies {count} count mem units from mem {src} to mem {dest} 
        /// using appropriate iteration. This method is safe even if src and dest overlaps.
        /// 
        /// @PARAM src: Mem block to copy from.
        /// @PARAM dest: Mem location to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr void CopyTo(const MemPtr src, usize count, MemPtr dest) const noexcept
        {
            ATOM_DEBUG_EXPECTS(src != nullptr);
            ATOM_DEBUG_EXPECTS(dest != nullptr);
            ATOM_DEBUG_EXPECTS(src != dest);

            if (dest > src)
            {
                _BwdCopy(src, count, dest);
            }
            else
            {
                _FwdCopy(src, count, dest);
            }
        }

    private:
        constexpr void _Fill(MemPtr mem, usize count, MemUnit val) const noexcept
        {
            std::fill(mem, mem + count, val);
        }
    
        constexpr void _FwdCopy(const MemPtr src, usize count, MemPtr dest) const noexcept
        {
            std::copy(src, src + count, dest);
        }

        constexpr void _BwdCopy(const MemPtr src, usize count, MemPtr dest) const noexcept
        {
            std::copy_backward(src, src + count, dest);
        }
    };
}