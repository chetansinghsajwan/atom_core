#pragma once
#include "Atom/Exceptions/Assertions.decl.h"

#include <algorithm>

#define ATOM_ATTR_UNUSED_PARAM(param) _unused_##param

#if ATOM_IS_CONFIG_DEBUG
    #define ATOM_ATTR_UNUSED_PARAM_DEBUG(param) ATOM_ATTR_UNUSED_PARAM(param)
    #define ATOM_ATTR_UNUSED_PARAM_RELEASE(param) param
#else
    #define ATOM_ATTR_UNUSED_PARAM_DEBUG(param) param
    #define ATOM_ATTR_UNUSED_PARAM_RELEASE(param) ATOM_ATTR_UNUSED_PARAM(param)
#endif

namespace Atom
{
    using MemUnit = byte;
    using MemPtr = byte*;

    struct MemBlk
    {
        constexpr MemBlk(MemPtr mem, usize count) noexcept:
            mem{ mem }, count{ count } { }

        constexpr MemBlk(MemPtr begin, MemPtr end) noexcept:
            mem{ begin }, count{ (usize)(end - begin) }
        {
            ATOM_DEBUG_EXPECTS(end >= begin);
        }

        MemPtr mem;
        usize count;
    };

    constexpr bool operator ==(const MemBlk& lhs, NullPtr) noexcept
    {
        return lhs.mem == nullptr && lhs.count > 0;
    }

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
        constexpr void Fill(MemBlk mem, MemUnit val) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            _Fill(mem.mem, mem.count, val);
        }

        /// ----------------------------------------------------------------------------------------
        /// Same as {Fill(...)}, but the call will not be optimized away.
        /// ----------------------------------------------------------------------------------------
        #pragma optimize("", off)
        constexpr void FillExplicit(MemBlk mem, MemUnit val) const noexcept
        {
            Fill(mem, val);
        }
        #pragma optimize("", on)

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {src} to mem block {dest} using fwd iteration.
        /// 
        /// @PARAM src: Mem block to copy from.
        /// @PARAM dest: Mem block to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr void FwdCopyTo(const MemBlk src, MemBlk dest) const noexcept
        {
            ATOM_DEBUG_EXPECTS(src != nullptr);
            ATOM_DEBUG_EXPECTS(dest != nullptr);

            ATOM_DEBUG_EXPECTS(dest.mem < src.mem) << ATOM_TEXT(
                "Src mem block overlaps with dest mem block.");

            _FwdCopy(src.mem, src.count, dest.mem);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {src} to mem block {dest} using bwd iteration.
        /// 
        /// @PARAM src: Mem block to copy from.
        /// @PARAM dest: Mem block to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr void BwdCopyTo(const MemBlk src, MemBlk dest) const noexcept
        {
            ATOM_DEBUG_EXPECTS(src != nullptr);
            ATOM_DEBUG_EXPECTS(dest != nullptr);

            ATOM_DEBUG_EXPECTS(dest.mem > (src.mem + src.count)) << ATOM_TEXT(
                "Src mem block overlaps with dest mem block.");

            _BwdCopy(src.mem, src.count, dest.mem);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {src} to mem block {dest} using appropriate 
        /// iteration. This method is safe even if {src} and {dest} overlaps.
        /// 
        /// @PARAM src: Mem block to copy from.
        /// @PARAM dest: Mem block to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr void CopyTo(const MemBlk src, MemBlk dest) const noexcept
        {
            ATOM_DEBUG_EXPECTS(src != nullptr);
            ATOM_DEBUG_EXPECTS(dest != nullptr);

            ATOM_DEBUG_EXPECTS(dest.count >= src.count);

            if (dest.mem > src.mem)
            {
                _BwdCopy(src.mem, src.count, dest.mem);
            }
            else if (dest.mem < src.mem)
            {
                _FwdCopy(src.mem, src.count, dest.mem);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {mem} to mem block outset by {outset}.
        /// 
        /// @PARAM mem: Mem block to copy from.
        /// @PARAM outset: Count of mem units outset from {mem}.
        /// ----------------------------------------------------------------------------------------
        constexpr void CopyFwd(const MemBlk mem, usize outset) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(outset > 0);

            _BwdCopy(mem.mem, mem.count, mem.mem + outset);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {mem} to mem block inset by {inset}.
        /// 
        /// @PARAM mem: Mem block to copy from.
        /// @PARAM inset: Count of mem units inset from {mem}.
        /// ----------------------------------------------------------------------------------------
        constexpr void CopyBwd(const MemBlk mem, usize inset) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(inset > 0);

            _FwdCopy(mem.mem, mem.count, mem.mem - inset);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {mem} to mem block offset by {offset}.
        /// This method is safe even if src and dest overlaps.
        /// 
        /// @PARAM mem: Mem block to copy from.
        /// @PARAM offset: Count of mem units offset from {mem}.
        /// ----------------------------------------------------------------------------------------
        constexpr void CopyBy(const MemBlk mem, isize offset) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(offset != 0);

            if (offset > 0)
            {
                _BwdCopy(mem.mem, mem.count, mem.mem + offset);
            }
            else
            {
                _FwdCopy(mem.mem, mem.count, mem.mem + offset);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Shifts each mem unit fwd in mem block {mem} by {steps} steps.
        /// 
        /// @PARAM mem: Mem block to shift mem units of.
        /// @PARAM steps: Count of mem units to shift by.
        /// ----------------------------------------------------------------------------------------
        constexpr void ShiftFwd(MemBlk mem, usize steps) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(steps > 0);

            _ShiftFwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// Shifts each mem unit bwd in mem block {mem} by {steps} steps.
        /// 
        /// @PARAM mem: Mem block to shift mem units of.
        /// @PARAM steps: Count of mem units to shift by.
        /// ----------------------------------------------------------------------------------------
        constexpr void ShiftBwd(MemBlk mem, usize steps) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(steps > 0);

            _ShiftBwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// Shifts each mem unit in mem block {mem} by {steps} steps.
        /// 
        /// @PARAM mem: Mem block to shift mem units of.
        /// @PARAM steps: Count of mem units to shift by.
        /// ----------------------------------------------------------------------------------------
        constexpr void ShiftBy(MemBlk mem, isize steps) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(steps != 0);

            if (steps > 0)
            {
                _ShiftFwd(mem.mem, mem.count, steps);
            }
            else
            {
                _ShiftBwd(mem.mem, mem.count, -steps);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Rotates mem block {mem} fwd by {steps} steps.
        /// 
        /// @PARAM mem: Mem block to rotate.
        /// @PARAM steps: Count of mem units to rotate by.
        /// ----------------------------------------------------------------------------------------
        constexpr void RotateFwd(MemBlk mem, usize steps) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(steps > 0);

            _RotateFwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// Rotates mem block {mem} bwd by {steps} steps.
        /// 
        /// @PARAM mem: Mem block to rotate.
        /// @PARAM steps: Count of mem units to rotate by.
        /// ----------------------------------------------------------------------------------------
        constexpr void RotateBwd(MemBlk mem, usize steps) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(steps > 0);

            _RotateBwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// Rotates mem block {mem} by {steps} steps.
        /// 
        /// @PARAM mem: Mem block to rotate.
        /// @PARAM steps: Count of mem units to rotate by.
        /// ----------------------------------------------------------------------------------------
        constexpr void RotateBy(MemBlk mem, isize steps) const noexcept
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);
            ATOM_DEBUG_EXPECTS(steps != 0);

            if (steps > 0)
            {
                _ShiftFwd(mem.mem, mem.count, steps);
            }
            else
            {
                _ShiftBwd(mem.mem, mem.count, -steps);
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

        constexpr void _ShiftFwd(const MemPtr mem, usize memCount, usize steps) const noexcept
        {
            std::shift_right(mem, mem + memCount, steps);
        }

        constexpr void _ShiftBwd(const MemPtr mem, usize memCount, usize steps) const noexcept
        {
            std::shift_left(mem, mem + memCount, steps);
        }

        constexpr void _RotateFwd(const MemPtr mem, usize memCount, usize offset) const noexcept
        {
            std::rotate(mem, mem + offset, mem + memCount);
        }

        constexpr void _RotateBwd(const MemPtr mem, usize memCount, usize offset) const noexcept
        {
            std::rotate(mem, mem + offset, mem + memCount);
        }
    };
}