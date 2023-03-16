#pragma once

extern "C"
{
    #include "WjCryptLib_Sha1.h"
}

#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// SHA1 Hash output.
    /// --------------------------------------------------------------------------------------------
    struct ATOM_API SHA1Hash
    {
        byte bytes[20];
    };

    /// --------------------------------------------------------------------------------------------
    /// SHA1 Hash Generator.
    /// --------------------------------------------------------------------------------------------
    class ATOM_API SHA1HashGenerator
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// 
        /// Same as calling Reset.
        /// ----------------------------------------------------------------------------------------
        SHA1HashGenerator() noexcept
        {
            Reset();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Resets the {SHA1HashGenerator} to its initial state.
        /// ----------------------------------------------------------------------------------------
        void Reset() noexcept
        {
            Sha1Initialise(&m_context);
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes data. This can be called infinite times.
        /// 
        /// @PARAM[IN] in_data Ptr to the input data.
        /// @PARAM[IN] in_data_size Size of the data.
        /// ----------------------------------------------------------------------------------------
        void ProcessBytes(const void* in_data, size_t in_data_size)
        {
            Sha1Update(&m_context, in_data, in_data_size);
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes single byte. This can be called infinite times.
        /// 
        /// @PARAM[IN] in_data Data to process.
        /// @PARAM[IN] in_data_size Size of the data.
        /// ----------------------------------------------------------------------------------------
        void ProcessByte(byte in_data)
        {
            Sha1Update(&m_context, &in_data, 1);
        }

        /// ----------------------------------------------------------------------------------------
        /// Generates SHA1 Hash.
        /// 
        /// @PARAM[OUT] out_hash Reference to {SHA1Hash} object to get output in.
        /// ----------------------------------------------------------------------------------------
        void Generate(SHA1Hash& out_hash) noexcept
        {
            Sha1Finalise(&m_context, reinterpret_cast<SHA1_HASH*>(out_hash.bytes));
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Underlying SHA1 implementation.
        /// ----------------------------------------------------------------------------------------
        Sha1Context m_context;
    };

    /// --------------------------------------------------------------------------------------------
    /// Generates SHA1 Hash using {SHA1HashGenerator}.
    /// --------------------------------------------------------------------------------------------
    inline SHA1Hash GENERATE_SHA1_HASH(const void* in_data, const size_t in_size)
    {
        SHA1HashGenerator generator;
        SHA1Hash hash;

        generator.ProcessBytes(in_data, in_size);
        generator.Generate(hash);

        return hash;
    }
}