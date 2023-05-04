#pragma once
#include "Atom/Hash/Private/T1Hash.h"
#include "Atom/Math.h"
#include "Atom/Exceptions.h"

namespace Atom::Private
{
    /// --------------------------------------------------------------------------------------------
    /// T1Hash Generator.
    /// --------------------------------------------------------------------------------------------
    template <typename T1Hash, typename TImpl>
    class T1HashGenerator
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// 
        /// Same as calling Reset.
        /// ----------------------------------------------------------------------------------------
        T1HashGenerator() noexcept
        {
            Reset();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Resets the {T1HashGenerator} to its initial state.
        /// ----------------------------------------------------------------------------------------
        void Reset() noexcept
        {
            _impl.Initialize();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInput, usize BufSize = 50>
        requires RInputIterator<TInput, byte>
        T1HashGenerator& ProcessBytes(TInput in)
        {
            if constexpr (RArrayIterator<TInput, byte>)
            {
                // TODO: Fix this.
                // return ProcessBytes(in.Data(), in.NextRange());
                return ProcessBytes(in.Data(), in.Range());
            }

            static_assert(BufSize <= NumLimits<uint32_t>::max() && BufSize > 10,
                "Keep {BufSize} smaller than or equal to max of {uint32_t}");

            usize count = 0;
            StaticArray<byte, BufSize> buf;

            for (byte b : in)
            {
                buf[count++] = b;
                if (count == BufSize || !in.HasNext())
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
        /// @PARAM[IN] data Ptr to the input data.
        /// @PARAM[IN] dataSize Size of the data.
        /// 
        /// @EXPECTS {data != nullptr}.
        /// @EXPECTS {dataSize > 0}.
        /// 
        /// @THROWS AssertionException Expects {data != nullptr}.
        /// @THROWS AssertionException Expects {dataSize > 0}.
        /// ----------------------------------------------------------------------------------------
        T1HashGenerator& ProcessBytes(const void* data, usize dataSize)
        {
            ATOM_DEBUG_ASSERT(data != nullptr);
            ATOM_DEBUG_ASSERT(dataSize > 0);

            // The implementation accepts size of data in uint32 but we accept in usize,
            // which could be bigger than uint32, so we pass the data to the implementation 
            // in parts.

            // Max size of input WjCrypt accepts at once.
            static constexpr usize maxInput = NumLimits<uint32_t>::max();
            for (usize processed = 0; processed < dataSize; processed += maxInput)
            {
                _impl.Update((byte*)data + processed,
                    (uint32_t)Math::Min(maxInput, dataSize - processed));
            }

            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes single byte. This can be called infinite times.
        /// 
        /// @PARAM[IN] data Data to process.
        /// ----------------------------------------------------------------------------------------
        T1HashGenerator& ProcessByte(byte data)
        {
            _impl.Update(&data, 1);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Generates T1Hash.
        /// 
        /// @RETURNS {T1Hash} object.
        /// ----------------------------------------------------------------------------------------
        T1Hash Generate() noexcept
        {
            T1Hash hash;
            _impl.Calculate(hash);
            return hash;
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Underlying T1HashGenerator implementation.
        /// ----------------------------------------------------------------------------------------
        TImpl _impl;
    };
}