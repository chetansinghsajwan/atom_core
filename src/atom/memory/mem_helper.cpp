module;
#include "atom/preprocessors.h"

export module atom.core:mem_helper;
import :core;
import :mem_ptr;
import :contracts_decl;
import :std;

export namespace atom
{
    class mem_blk
    {
    public:
        constexpr mem_blk(mut_mem_ptr<void> mem, usize count)
            : mem(mem)
            , count(count)
        {}

        constexpr mem_blk(mut_mem_ptr<void> begin, mut_mem_ptr<void> end)
            : mem(begin)
            , count(usize(end - begin))
        {
            contracts::debug_expects(end >= begin);
        }

    public:
        mut_mem_ptr<void> mem;
        usize count;
    };

    constexpr auto operator==(const mem_blk& lhs, nullptr_type) -> bool
    {
        return lhs.mem == nullptr && lhs.count > 0;
    }

    /// --------------------------------------------------------------------------------------------
    /// contains basic memory utility functions.
    /// --------------------------------------------------------------------------------------------
    class mem_helper
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// sets each mem unit of mem block {mem} with value {val}.
        ///
        /// @param mem: mem block to write to.
        /// @param val: value to write.
        /// ----------------------------------------------------------------------------------------
        constexpr auto fill(mem_blk mem, byte val) const
        {
            contracts::debug_expects(mem != nullptr);

            _fill(mem.mem, mem.count, val);
        }

        /// ----------------------------------------------------------------------------------------
        /// same as {fill(...)}, but the call will not be optimized away.
        /// ----------------------------------------------------------------------------------------
        ATOM_PRAGMA_OPTIMIZE_OFF

        constexpr auto fill_explicit(mem_blk mem, byte val) const
        {
            fill(mem, val);
        }

        ATOM_PRAGMA_OPTIMIZE_ON

        /// ----------------------------------------------------------------------------------------
        /// copies each mem unit from mem block {src} to mem block {dest} using fwd iteration.
        ///
        /// @param src: mem block to copy from.
        /// @param dest: mem block to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr auto fwd_copy_to(const mem_blk src, mem_blk dest) const
        {
            contracts::debug_expects(src != nullptr);
            contracts::debug_expects(dest != nullptr);

            contracts::debug_expects(
                dest.mem < src.mem, "src mem block overlaps with dest mem block.");

            _fwd_copy(src.mem, src.count, dest.mem);
        }

        /// ----------------------------------------------------------------------------------------
        /// copies each mem unit from mem block {src} to mem block {dest} using bwd iteration.
        ///
        /// @param src: mem block to copy from.
        /// @param dest: mem block to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr auto bwd_copy_to(const mem_blk src, mem_blk dest) const
        {
            contracts::debug_expects(src != nullptr);
            contracts::debug_expects(dest != nullptr);

            contracts::debug_expects(
                dest.mem > (src.mem + src.count), "src mem block overlaps with dest mem block.");

            _bwd_copy(src.mem, src.count, dest.mem);
        }

        /// ----------------------------------------------------------------------------------------
        /// copies each mem unit from mem block {src} to mem block {dest} using appropriate
        /// iteration. this_type method is safe even if {src} and {dest} overlaps.
        ///
        /// @param src: mem block to copy from.
        /// @param dest: mem block to copy to.
        /// ----------------------------------------------------------------------------------------
        constexpr auto copy_to(const mem_blk src, mem_blk dest) const
        {
            contracts::debug_expects(src != nullptr);
            contracts::debug_expects(dest != nullptr);

            contracts::debug_expects(dest.count >= src.count);

            if (dest.mem > src.mem)
            {
                _bwd_copy(src.mem, src.count, dest.mem);
            }
            else if (dest.mem < src.mem)
            {
                _fwd_copy(src.mem, src.count, dest.mem);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// copies each mem unit from mem block {mem} to mem block outset by {outset}.
        ///
        /// @param mem: mem block to copy from.
        /// @param outset: count of mem units outset from {mem}.
        /// ----------------------------------------------------------------------------------------
        constexpr auto copy_fwd(const mem_blk mem, usize outset) const
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(outset > 0);

            _bwd_copy(mem.mem, mem.count, mem.mem + outset);
        }

        /// ----------------------------------------------------------------------------------------
        /// copies each mem unit from mem block {mem} to mem block inset by {inset}.
        ///
        /// @param mem: mem block to copy from.
        /// @param inset: count of mem units inset from {mem}.
        /// ----------------------------------------------------------------------------------------
        constexpr auto copy_bwd(const mem_blk mem, usize inset) const
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(inset > 0);

            _fwd_copy(mem.mem, mem.count, mem.mem - inset);
        }

        /// ----------------------------------------------------------------------------------------
        /// copies each mem unit from mem block {mem} to mem block offset by {offset}.
        /// this_type method is safe even if src and dest overlaps.
        ///
        /// @param mem: mem block to copy from.
        /// @param offset: count of mem units offset from {mem}.
        /// ----------------------------------------------------------------------------------------
        constexpr auto copy_by(const mem_blk mem, isize offset) const
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(offset != 0);

            if (offset > 0)
            {
                _bwd_copy(mem.mem, mem.count, mem.mem + offset);
            }
            else
            {
                _fwd_copy(mem.mem, mem.count, mem.mem + offset);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// shifts each mem unit fwd in mem block {mem} by {steps} steps.
        ///
        /// @param mem: mem block to shift mem units of.
        /// @param steps: count of mem units to shift by.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_fwd(mem_blk mem, usize steps) const
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps > 0);

            _shift_fwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// shifts each mem unit bwd in mem block {mem} by {steps} steps.
        ///
        /// @param mem: mem block to shift mem units of.
        /// @param steps: count of mem units to shift by.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_bwd(mem_blk mem, usize steps) const
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps > 0);

            _shift_bwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// shifts each mem unit in mem block {mem} by {steps} steps.
        ///
        /// @param mem: mem block to shift mem units of.
        /// @param steps: count of mem units to shift by.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_by(mem_blk mem, isize steps) const
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps != 0);

            if (steps > 0)
            {
                _shift_fwd(mem.mem, mem.count, steps.to<usize>());
            }
            else
            {
                _shift_bwd(mem.mem, mem.count, steps.abs().to<usize>());
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// rotates mem block {mem} fwd by {steps} steps.
        ///
        /// @param mem: mem block to rotate.
        /// @param steps: count of mem units to rotate by.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_fwd(mem_blk mem, usize steps) const
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps > 0);

            _rotate_fwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// rotates mem block {mem} bwd by {steps} steps.
        ///
        /// @param mem: mem block to rotate.
        /// @param steps: count of mem units to rotate by.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_bwd(mem_blk mem, usize steps) const
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps > 0);

            _rotate_bwd(mem.mem, mem.count, steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// rotates mem block {mem} by {steps} steps.
        ///
        /// @param mem: mem block to rotate.
        /// @param steps: count of mem units to rotate by.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_by(mem_blk mem, isize steps) const
        {
            contracts::debug_expects(mem != nullptr);
            contracts::debug_expects(steps != 0);

            if (steps > 0)
            {
                _shift_fwd(mem.mem, mem.count, steps.to<usize>());
            }
            else
            {
                _shift_bwd(mem.mem, mem.count, steps.abs().to<usize>());
            }
        }

    private:
        constexpr auto _fill(mut_mem_ptr<void> mem, usize count, byte val) const -> void
        {
            std::fill(mem.unwrap_as_byte(), (mem + count).unwrap_as_byte(), val);
        }

        constexpr auto _fwd_copy(mem_ptr<void> src, usize count, mut_mem_ptr<void> dest) const
            -> void
        {
            std::copy(src.unwrap_as_byte(), (src + count).unwrap_as_byte(), dest.unwrap_as_byte());
        }

        constexpr auto _bwd_copy(mem_ptr<void> src, usize count, mut_mem_ptr<void> dest) const
            -> void
        {
            std::copy_backward(
                src.unwrap_as_byte(), (src + count).unwrap_as_byte(), dest.unwrap_as_byte());
        }

        constexpr auto _shift_fwd(mut_mem_ptr<void> mem, usize mem_count, usize steps) const -> void
        {
            std::shift_right(
                mem.unwrap_as_byte(), (mem + mem_count).unwrap_as_byte(), steps.unwrap());
        }

        constexpr auto _shift_bwd(mut_mem_ptr<void> mem, usize mem_count, usize steps) const -> void
        {
            std::shift_left(
                mem.unwrap_as_byte(), (mem + mem_count).unwrap_as_byte(), steps.unwrap());
        }

        constexpr auto _rotate_fwd(mut_mem_ptr<void> mem, usize mem_count, usize offset) const
            -> void
        {
            std::rotate(mem.unwrap_as_byte(), (mem + offset).unwrap_as_byte(),
                (mem + mem_count).unwrap_as_byte());
        }

        constexpr auto _rotate_bwd(mut_mem_ptr<void> mem, usize mem_count, usize offset) const
            -> void
        {
            std::rotate(mem.unwrap_as_byte(), (mem + offset).unwrap_as_byte(),
                (mem + mem_count).unwrap_as_byte());
        }
    };
}
