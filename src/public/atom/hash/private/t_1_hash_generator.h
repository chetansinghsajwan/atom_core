#pragma once
#include "Atom/Exceptions.h"
#include "Atom/Hash/Private/T1Hash.h"
#include "Atom/Math.h"

namespace Atom::Private
{
    /// --------------------------------------------------------------------------------------------
    /// T1Hash Generator.
    ///
    /// # To Do
    ///
    /// - Make this type constexpr.
    /// --------------------------------------------------------------------------------------------
    template <typename T1Hash, typename TImpl>
    class T1HashGenerator
    {
        using Self = T1HashGenerator;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # DefaultConstructor.
        ///
        /// Same as calling Reset.
        /// ----------------------------------------------------------------------------------------
        constexpr T1HashGenerator()
        {
            Reset();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Resets the `T1HashGenerator` to its initial state.
        /// ----------------------------------------------------------------------------------------
        constexpr auto Reset()
        {
            _impl.Initialize();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, usize BufSize = 50>
        constexpr auto ProcessBytes(TRange bytes) -> Self&
            requires RRangeOf<TRange, byte>
        {
            if constexpr (RArrayRangeOf<TRange, byte>)
            {
                return ProcessBytes(bytes.data(), bytes.Size());
            }

            static_assert(BufSize <= u32::Max() && BufSize > 10,
                "Keep {BufSize} smaller than or equal to max of {uint32_t}");

            usize count = 0;
            StaticArray<byte, BufSize> buf;

            auto end = bytes.End();
            for (auto it = bytes.Begin(); it != end; it++)
            {
                buf[count++] = *it;
                if (count == BufSize || it == end)
                {
                    _impl.Update(buf.data(), count);
                    count = 0;
                }
            }

            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes data. This can be called infinite times.
        ///
        /// # Parameters
        ///
        /// - `data`: MutPtr to the input data.
        /// - `dataSize`: Size of the data.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ProcessBytes(MemPtr<void> data, usize dataSize) -> Self&
        {
            Contracts::DebugExpects(data != nullptr);
            Contracts::DebugExpects(dataSize > 0);

            // The implementation accepts size of data in u32 but we accept in usize,
            // which could be bigger than u32, so we pass the data to the implementation
            // in parts.

            // Max size of input WjCrypt accepts at once.
            constexpr u32 maxInput = u32::Max();
            for (usize processed = 0; processed < dataSize; processed += maxInput)
            {
                _impl.Update(
                    data + processed, maxInput.min(dataSize - processed).to<u32>().unwrap());
            }

            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes single byte. This can be called infinite times.
        ///
        /// # Parameters
        ///
        /// - `data`: Data to process.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ProcessByte(byte data) -> Self&
        {
            _impl.Update(&data, 1);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Generates T1Hash.
        ///
        /// # Returns
        ///
        /// `T1Hash` object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto Generate() -> T1Hash
        {
            T1Hash hash;
            _impl.Calculate(hash);
            return hash;
        }

    private:
        TImpl _impl;
    };
}
