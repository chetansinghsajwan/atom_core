#pragma once
#include "Atom/Exceptions/Assertions.decl.h"

#include <algorithm>

namespace Atom
{
    using MemUnit = byte;
    using MemPtr = byte*;

    class MemBlk
    {
    public:
        constexpr MemBlk(MemPtr mem, usize count):
            mem{ mem }, count{ count } { }

        constexpr MemBlk(MemPtr begin, MemPtr end):
            mem{ begin }, count{ (usize)(end - begin) }
        {
            ATOM_DEBUG_EXPECTS(end >= begin);
        }

    public:
        MemPtr mem;
        usize count;
    };

    constexpr fn operator==(const MemBlk& lhs, NullPtr) -> bool
    {
        return lhs.mem == nullptr && lhs.count > 0;
    }

    /// --------------------------------------------------------------------------------------------
    /// Contains basic memory utility functions.
    /// --------------------------------------------------------------------------------------------
    class MemHelper
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Sets each mem unit of mem block {mem} with value {val}.
        /// 
        /// @PARAM mem: Mem block to write to.
        /// @PARAM val: Value to write.
        /// ----------------------------------------------------------------------------------------
        constexpr fn Fill(MemBlk mem, MemUnit val) const
        {
            ATOM_DEBUG_EXPECTS(mem != nullptr);

            _Fill(mem.mem, mem.count, val);
        }

        /// ----------------------------------------------------------------------------------------
        /// Same as {Fill(...)}, but the call will not be optimized away.
        /// ----------------------------------------------------------------------------------------
        #pragma optimize("", off)
        constexpr fn FillExplicit(MemBlk mem, MemUnit val) const
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
        constexpr fn FwdCopyTo(const MemBlk src, MemBlk dest) const
        {
            ATOM_DEBUG_EXPECTS(src != nullptr);
            ATOM_DEBUG_EXPECTS(dest != nullptr);

            ATOM_DEBUG_EXPECTS(dest.mem < src.mem) <<
                "Src mem block overlaps with dest mem block.";

            _FwdCopy(src.mem, src.count, dest.mem);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {src} to mem block {dest} using bwd iteration.
        /// 
        /// @PARAM src: Mem block to copy from.
        /// @PARAM dest: Mem block to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr fn BwdCopyTo(const MemBlk src, MemBlk dest) const
        {
            ATOM_DEBUG_EXPECTS(src != nullptr);
            ATOM_DEBUG_EXPECTS(dest != nullptr);

            ATOM_DEBUG_EXPECTS(dest.mem > (src.mem + src.count)) <<
                "Src mem block overlaps with dest mem block.";

            _BwdCopy(src.mem, src.count, dest.mem);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {src} to mem block {dest} using appropriate 
        /// iteration. This method is safe even if {src} and {dest} overlaps.
        /// 
        /// @PARAM src: Mem block to copy from.
        /// @PARAM dest: Mem block to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr fn CopyTo(const MemBlk src, MemBlk dest) const
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
        constexpr fn CopyFwd(const MemBlk mem, usize outset) const
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
        constexpr fn CopyBwd(const MemBlk mem, usize inset) const
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
        constexpr fn CopyBy(const MemBlk mem, isize offset) const
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
        constexpr fn ShiftFwd(MemBlk mem, usize steps) const
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
        constexpr fn ShiftBwd(MemBlk mem, usize steps) const
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
        constexpr fn ShiftBy(MemBlk mem, isize steps) const
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
        constexpr fn RotateFwd(MemBlk mem, usize steps) const
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
        constexpr fn RotateBwd(MemBlk mem, usize steps) const
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
        constexpr fn RotateBy(MemBlk mem, isize steps) const
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
        constexpr fn _Fill(MemPtr mem, usize count, MemUnit val) const -> void
        {
            std::fill(mem, mem + count, val);
        }
    
        constexpr fn _FwdCopy(const MemPtr src, usize count, MemPtr dest) const -> void
        {
            std::copy(src, src + count, dest);
        }

        constexpr fn _BwdCopy(const MemPtr src, usize count, MemPtr dest) const -> void
        {
            std::copy_backward(src, src + count, dest);
        }

        constexpr fn _ShiftFwd(const MemPtr mem, usize memCount, usize steps) const -> void
        {
            std::shift_right(mem, mem + memCount, steps);
        }

        constexpr fn _ShiftBwd(const MemPtr mem, usize memCount, usize steps) const -> void
        {
            std::shift_left(mem, mem + memCount, steps);
        }

        constexpr fn _RotateFwd(const MemPtr mem, usize memCount, usize offset) const -> void
        {
            std::rotate(mem, mem + offset, mem + memCount);
        }

        constexpr fn _RotateBwd(const MemPtr mem, usize memCount, usize offset) const -> void
        {
            std::rotate(mem, mem + offset, mem + memCount);
        }
    };
}