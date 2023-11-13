#pragma once
#include "Atom/Contracts.h"

#include <algorithm>

namespace Atom
{
    class MemBlk
    {
    public:
        constexpr MemBlk(MemPtr<void> mem, usize count):
            mem{ mem }, count{ count }
        {}

        constexpr MemBlk(MemPtr<void> begin, MemPtr<void> end):
            mem{ begin }, count{ end - begin }
        {
            debug_expects(end >= begin);
        }

    public:
        MemPtr<void> mem;
        usize count;
    };

    constexpr auto operator==(const MemBlk& lhs, NullPtr) -> bool
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
        constexpr auto Fill(MemBlk mem, memunit val) const
        {
            debug_expects(mem != nullptr);

            _Fill(mem.mem, mem.count, val);
        }

        /// ----------------------------------------------------------------------------------------
        /// Same as {Fill(...)}, but the call will not be optimized away.
        /// ----------------------------------------------------------------------------------------
        ATOM_PRAGMA_OPTIMIZE_OFF

        constexpr auto FillExplicit(MemBlk mem, memunit val) const
        {
            Fill(mem, val);
        }

        ATOM_PRAGMA_OPTIMIZE_ON

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {src} to mem block {dest} using fwd iteration.
        ///
        /// @PARAM src: Mem block to copy from.
        /// @PARAM dest: Mem block to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr auto FwdCopyTo(const MemBlk src, MemBlk dest) const
        {
            debug_expects(src != nullptr);
            debug_expects(dest != nullptr);

            debug_expects(dest.mem < src.mem, "Src mem block overlaps with dest mem block.");

            _FwdCopy(src.mem, src.count, dest.mem);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {src} to mem block {dest} using bwd iteration.
        ///
        /// @PARAM src: Mem block to copy from.
        /// @PARAM dest: Mem block to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr auto BwdCopyTo(const MemBlk src, MemBlk dest) const
        {
            debug_expects(src != nullptr);
            debug_expects(dest != nullptr);

            debug_expects(
                dest.mem > (src.mem + src.count), "Src mem block overlaps with dest mem block.");

            _BwdCopy(src.mem, src.count, dest.mem);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {src} to mem block {dest} using appropriate
        /// iteration. This method is safe even if {src} and {dest} overlaps.
        ///
        /// @PARAM src: Mem block to copy from.
        /// @PARAM dest: Mem block to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr auto CopyTo(const MemBlk src, MemBlk dest) const
        {
            debug_expects(src != nullptr);
            debug_expects(dest != nullptr);

            debug_expects(dest.count >= src.count);

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
        constexpr auto CopyFwd(const MemBlk mem, usize outset) const
        {
            debug_expects(mem != nullptr);
            debug_expects(outset > 0);

            _BwdCopy(mem.mem, mem.count, mem.mem + outset);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {mem} to mem block inset by {inset}.
        ///
        /// @PARAM mem: Mem block to copy from.
        /// @PARAM inset: Count of mem units inset from {mem}.
        /// ----------------------------------------------------------------------------------------
        constexpr auto CopyBwd(const MemBlk mem, usize inset) const
        {
            debug_expects(mem != nullptr);
            debug_expects(inset > 0);

            _FwdCopy(mem.mem, mem.count, mem.mem - inset);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies each mem unit from mem block {mem} to mem block offset by {offset}.
        /// This method is safe even if src and dest overlaps.
        ///
        /// @PARAM mem: Mem block to copy from.
        /// @PARAM offset: Count of mem units offset from {mem}.
        /// ----------------------------------------------------------------------------------------
        constexpr auto CopyBy(const MemBlk mem, isize offset) const
        {
            debug_expects(mem != nullptr);
            debug_expects(offset != 0);

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
        constexpr auto ShiftFwd(MemBlk mem, usize steps) const
        {
            debug_expects(mem != nullptr);
            debug_expects(steps > 0);

            _ShiftFwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// Shifts each mem unit bwd in mem block {mem} by {steps} steps.
        ///
        /// @PARAM mem: Mem block to shift mem units of.
        /// @PARAM steps: Count of mem units to shift by.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ShiftBwd(MemBlk mem, usize steps) const
        {
            debug_expects(mem != nullptr);
            debug_expects(steps > 0);

            _ShiftBwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// Shifts each mem unit in mem block {mem} by {steps} steps.
        ///
        /// @PARAM mem: Mem block to shift mem units of.
        /// @PARAM steps: Count of mem units to shift by.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ShiftBy(MemBlk mem, isize steps) const
        {
            debug_expects(mem != nullptr);
            debug_expects(steps != 0);

            if (steps > 0)
            {
                _ShiftFwd(mem.mem, mem.count, steps.to<usize>());
            }
            else
            {
                _ShiftBwd(mem.mem, mem.count, steps.abs().to<usize>());
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Rotates mem block {mem} fwd by {steps} steps.
        ///
        /// @PARAM mem: Mem block to rotate.
        /// @PARAM steps: Count of mem units to rotate by.
        /// ----------------------------------------------------------------------------------------
        constexpr auto RotateFwd(MemBlk mem, usize steps) const
        {
            debug_expects(mem != nullptr);
            debug_expects(steps > 0);

            _RotateFwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// Rotates mem block {mem} bwd by {steps} steps.
        ///
        /// @PARAM mem: Mem block to rotate.
        /// @PARAM steps: Count of mem units to rotate by.
        /// ----------------------------------------------------------------------------------------
        constexpr auto RotateBwd(MemBlk mem, usize steps) const
        {
            debug_expects(mem != nullptr);
            debug_expects(steps > 0);

            _RotateBwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// Rotates mem block {mem} by {steps} steps.
        ///
        /// @PARAM mem: Mem block to rotate.
        /// @PARAM steps: Count of mem units to rotate by.
        /// ----------------------------------------------------------------------------------------
        constexpr auto RotateBy(MemBlk mem, isize steps) const
        {
            debug_expects(mem != nullptr);
            debug_expects(steps != 0);

            if (steps > 0)
            {
                _ShiftFwd(mem.mem, mem.count, steps.to<usize>());
            }
            else
            {
                _ShiftBwd(mem.mem, mem.count, steps.abs().to<usize>());
            }
        }

    private:
        constexpr auto _Fill(MemPtr<void> mem, usize count, memunit val) const -> void
        {
            std::fill(mem.unwrapAsByte(), (mem + count).unwrapAsByte(), val);
        }

        constexpr auto _FwdCopy(ConstMemPtr<void> src, usize count, MemPtr<void> dest) const -> void
        {
            std::copy(src.unwrapAsByte(), (src + count).unwrapAsByte(), dest.unwrapAsByte());
        }

        constexpr auto _BwdCopy(ConstMemPtr<void> src, usize count, MemPtr<void> dest) const -> void
        {
            std::copy_backward(src.unwrapAsByte(), (src + count).unwrapAsByte(), dest.unwrapAsByte());
        }

        constexpr auto _ShiftFwd(MemPtr<void> mem, usize memCount, usize steps) const -> void
        {
            std::shift_right(mem.unwrapAsByte(), (mem + memCount).unwrapAsByte(), steps.unwrap());
        }

        constexpr auto _ShiftBwd(MemPtr<void> mem, usize memCount, usize steps) const -> void
        {
            std::shift_left(mem.unwrapAsByte(), (mem + memCount).unwrapAsByte(), steps.unwrap());
        }

        constexpr auto _RotateFwd(MemPtr<void> mem, usize memCount, usize offset) const -> void
        {
            std::rotate(mem.unwrapAsByte(), (mem + offset).unwrapAsByte(), (mem + memCount).unwrapAsByte());
        }

        constexpr auto _RotateBwd(MemPtr<void> mem, usize memCount, usize offset) const -> void
        {
            std::rotate(mem.unwrapAsByte(), (mem + offset).unwrapAsByte(), (mem + memCount).unwrapAsByte());
        }
    };
}
