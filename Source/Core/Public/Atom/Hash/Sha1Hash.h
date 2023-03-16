#pragma once

extern "C"
{
    #include "WjCryptLib_Sha1.h"
}

#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Sha1 Hash output.
    /// --------------------------------------------------------------------------------------------
    struct ATOM_API Sha1Hash
    {
        byte bytes[20];
    };

    /// --------------------------------------------------------------------------------------------
    /// Sha1 Hash Generator.
    /// --------------------------------------------------------------------------------------------
    class ATOM_API Sha1HashGenerator
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
        /// Generates Sha1 Hash.
        /// 
        /// @PARAM[OUT] out_hash Reference to {Sha1Hash} object to get output in.
        /// ----------------------------------------------------------------------------------------
        void Generate(Sha1Hash& out_hash) noexcept
        {
            Sha1Finalise(&m_context, reinterpret_cast<SHA1_HASH*>(out_hash.bytes));
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
    inline Sha1Hash GENERATE_Sha1_HASH(const void* in_data, const size_t in_size)
    {
        Sha1HashGenerator generator;
        Sha1Hash hash;

        generator.ProcessBytes(in_data, in_size);
        generator.Generate(hash);

        return hash;
    }
}