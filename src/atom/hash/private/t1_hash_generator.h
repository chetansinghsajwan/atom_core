#pragma once
#include "atom/exceptions.h"
#include "atom/hash/private/t1_hash.h"
#include "atom/math.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// t1_hash generator.
    ///
    /// # to do
    ///
    /// - make this type constexpr.
    /// --------------------------------------------------------------------------------------------
    template <typename t1_hash, typename impl_type>
    class _t1_hash_generator
    {
        using self = _t1_hash_generator;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default_constructor.
        ///
        /// same as calling reset.
        /// ----------------------------------------------------------------------------------------
        constexpr _t1_hash_generator()
        {
            reset();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// resets the `_t1_hash_generator` to its initial state.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reset()
        {
            _impl.initialize();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename range_type, usize buf_size = 50>
        constexpr auto process_bytes(range_type bytes) -> self&
            requires rrange_of<range_type, byte>
        {
            if constexpr (rarray_range_of<range_type, byte>)
            {
                return process_bytes(bytes.data(), bytes.size());
            }

            static_assert(buf_size <= u32::max() && buf_size > 10,
                "keep {buf_size} smaller than or equal to max of {uint32_t}");

            usize count = 0;
            static_array<byte, buf_size> buf;

            auto end = bytes.end();
            for (auto it = bytes.begin(); it != end; it++)
            {
                buf[count++] = *it;
                if (count == buf_size || it == end)
                {
                    _impl.update(buf.data(), count);
                    count = 0;
                }
            }

            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// processes data. this_type can be called infinite times.
        ///
        /// # parameters
        ///
        /// - `data`: mut_ptr to the input data.
        /// - `data_size`: size of the data.
        /// ----------------------------------------------------------------------------------------
        constexpr auto process_bytes(mem_ptr<void> data, usize data_size) -> self&
        {
            contracts::debug_expects(data != nullptr);
            contracts::debug_expects(data_size > 0);

            // the implementation accepts size of data in u32 but we accept in usize,
            // which could be bigger than u32, so we pass the data to the implementation
            // in parts.

            // max size of input wj_crypt accepts at once.
            constexpr u32 max_input = u32::max();
            for (usize processed = 0; processed < data_size; processed += max_input)
            {
                _impl.update(
                    data + processed, max_input.min(data_size - processed).to<u32>().unwrap());
            }

            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// processes single byte. this_type can be called infinite times.
        ///
        /// # parameters
        ///
        /// - `data`: data to process.
        /// ----------------------------------------------------------------------------------------
        constexpr auto process_byte(byte data) -> self&
        {
            _impl.update(&data, 1);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// generates t1_hash.
        ///
        /// # returns
        ///
        /// `t1_hash` object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto generate() -> t1_hash
        {
            t1_hash hash;
            _impl.calculate(hash);
            return hash;
        }

    private:
        impl_type _impl;
    };
}
