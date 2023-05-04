#pragma once
extern "C"
{
#include "WjCryptLib_Sha1.h"
}

#include "Sha1Hash.h"
#include "Atom/Math.h"
#include "Atom/Exceptions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Sha1 Hash Generator.
    /// --------------------------------------------------------------------------------------------
    class Sha1HashGenerator
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// 
        /// Same as calling Reset.
        /// ----------------------------------------------------------------------------------------
        Sha1HashGenerator() noexcept
        {
            Reset();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Resets the {Sha1HashGenerator} to its initial state.
        /// ----------------------------------------------------------------------------------------
        void Reset() noexcept
        {
            Sha1Initialise(&m_context);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInput, usize BufSize = 50>
        requires RInputIterator<TInput, byte>
        Sha1HashGenerator& ProcessBytes(TInput in)
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
                    Sha1Update(&m_context, buf.data(), count);
                    count = 0;
                }
            }

            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes data. This can be called infinite times.
        /// 
        /// @PARAM[IN] in_data Ptr to the input data.
        /// @PARAM[IN] in_dataSize Size of the data.
        /// 
        /// @EXPECTS {in_data != nullptr}.
        /// @EXPECTS {in_dataSize > 0}.
        /// 
        /// @THROWS AssertionException Expects {in_data != nullptr}.
        /// @THROWS AssertionException Expects {in_dataSize > 0}.
        /// ----------------------------------------------------------------------------------------
        Sha1HashGenerator& ProcessBytes(const void* in_data, usize in_dataSize)
        {
            ATOM_DEBUG_ASSERT(in_data != nullptr);
            ATOM_DEBUG_ASSERT(in_dataSize > 0);

            // The implementation accepts size of data in uint32 but we accept in usize,
            // which could be bigger than uint32, so we pass the data to the implementation 
            // in parts.

            // Max size of input WjCrypt accepts at once.
            static constexpr usize maxInput = NumLimits<uint32_t>::max();
            for (usize processed = 0; processed < in_dataSize; processed += maxInput)
            {
                Sha1Update(&m_context, (byte*)in_data + processed,
                    Math::Min(maxInput, in_dataSize - processed));
            }

            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes single byte. This can be called infinite times.
        /// 
        /// @PARAM[IN] in_data Data to process.
        /// ----------------------------------------------------------------------------------------
        Sha1HashGenerator& ProcessByte(byte in_data)
        {
            Sha1Update(&m_context, &in_data, 1);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Generates Sha1 Hash.
        /// 
        /// @RETURNS {Sha1Hash} object.
        /// ----------------------------------------------------------------------------------------
        Sha1Hash Generate() noexcept
        {
            Sha1Hash hash;
            Sha1Finalise(&m_context, (SHA1_HASH*)hash.bytes.data());
            return hash;
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Underlying Sha1 implementation.
        /// ----------------------------------------------------------------------------------------
        Sha1Context m_context;
    };
}