#pragma once

extern "C"
{
    #include "WjCryptLib_Md5.h"
}

#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Md5 Hash output.
    /// --------------------------------------------------------------------------------------------
    struct ATOM_API Md5Hash
    {
        byte bytes[16];
    };

    /// --------------------------------------------------------------------------------------------
    /// Md5 Hash Generator.
    /// --------------------------------------------------------------------------------------------
    class ATOM_API Md5HashGenerator
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// 
        /// Same as calling Reset.
        /// ----------------------------------------------------------------------------------------
        Md5HashGenerator() noexcept
        {
            Reset();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Resets the {Md5HashGenerator} to its initial state.
        /// ----------------------------------------------------------------------------------------
        void Reset() noexcept
        {
            Md5Initialise(&m_context);
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes data. This can be called infinite times.
        /// 
        /// @PARAM[IN] in_data Ptr to the input data.
        /// @PARAM[IN] in_data_size Size of the data.
        /// ----------------------------------------------------------------------------------------
        void ProcessBytes(const void* in_data, size_t in_data_size)
        {
            Md5Update(&m_context, in_data, in_data_size);
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes single byte. This can be called infinite times.
        /// 
        /// @PARAM[IN] in_data Data to process.
        /// @PARAM[IN] in_data_size Size of the data.
        /// ----------------------------------------------------------------------------------------
        void ProcessByte(byte in_data)
        {
            Md5Update(&m_context, &in_data, 1);
        }

        /// ----------------------------------------------------------------------------------------
        /// Generates Md5 Hash.
        /// 
        /// @PARAM[OUT] out_hash Reference to {Md5Hash} object to get output in.
        /// ----------------------------------------------------------------------------------------
        void Generate(Md5Hash& out_hash) noexcept
        {
            Md5Finalise(&m_context, reinterpret_cast<MD5_HASH*>(out_hash.bytes));
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Underlying Md5 implementation.
        /// ----------------------------------------------------------------------------------------
        Md5Context m_context;
    };

    /// --------------------------------------------------------------------------------------------
    /// Generates Md5 Hash using {Md5HashGenerator}.
    /// --------------------------------------------------------------------------------------------
    inline Md5Hash GENERATE_Md5_HASH(const void* in_data, const size_t in_size)
    {
        Md5HashGenerator generator;
        Md5Hash hash;

        generator.ProcessBytes(in_data, in_size);
        generator.Generate(hash);

        return hash;
    }
}