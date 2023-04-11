#pragma once

extern "C"
{
    #include "WjCryptLib_Sha1.h"
}

#include "Atom/Core.h"
#include "Atom/Exceptions.h"
#include "Atom/Math.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Sha1 Hash output.
    /// --------------------------------------------------------------------------------------------
    struct Sha1Hash
    {
        byte bytes[20];
    };

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
        void ProcessBytes(const void* in_data, usize in_dataSize)
        {
            ATOM_DEBUG_ASSERT(in_data != nullptr);
            ATOM_DEBUG_ASSERT(in_dataSize > 0);

            // Max size of input allowed at once.
            static constexpr usize maxInput = NumLimits<uint32_t>::max();

            const byte* data = (byte*)in_data;

            // Process data in blocks.
            for (usize i = 0; in_dataSize > 0; i++)
            {
                if (in_dataSize <= maxInput)
                {
                    Sha1Update(&m_context, data + (i * maxInput), (uint32_t)in_dataSize);
                }
                else
                {
                    Sha1Update(&m_context, data + (i * maxInput), maxInput);
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes single byte. This can be called infinite times.
        /// 
        /// @PARAM[IN] in_data Data to process.
        /// ----------------------------------------------------------------------------------------
        void ProcessByte(byte in_data)
        {
            Sha1Update(&m_context, &in_data, 1);
        }

        /// ----------------------------------------------------------------------------------------
        /// Generates Sha1 Hash.
        /// 
        /// @RETURNS {Sha1Hash} object.
        /// ----------------------------------------------------------------------------------------
        Sha1Hash Generate() noexcept
        {
            Sha1Hash hash;
            Sha1Finalise(&m_context, reinterpret_cast<SHA1_HASH*>(hash.bytes));
            return hash;
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Underlying Sha1 implementation.
        /// ----------------------------------------------------------------------------------------
        Sha1Context m_context;
    };

    /// --------------------------------------------------------------------------------------------
    /// Generates Sha1 Hash using {Sha1HashGenerator}.
    /// --------------------------------------------------------------------------------------------
    inline Sha1Hash GENERATE_Sha1_HASH(const void* in_data, const usize in_dataSize)
    {
        Sha1HashGenerator generator;

        generator.ProcessBytes(in_data, in_dataSize);
        return generator.Generate();
    }
}