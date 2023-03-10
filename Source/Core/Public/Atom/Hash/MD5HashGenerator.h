#pragma once

extern "C"
{
    #include "WjCryptLib_Md5.h"
}

#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// MD5 Hash output.
    /// --------------------------------------------------------------------------------------------
    struct ATOM_API MD5Hash
    {
        byte bytes[16];
    };

    /// --------------------------------------------------------------------------------------------
    /// MD5 Hash Generator.
    /// --------------------------------------------------------------------------------------------
    class ATOM_API MD5HashGenerator
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// 
        /// Same as calling Reset.
        /// ----------------------------------------------------------------------------------------
        MD5HashGenerator() noexcept
        {
            Reset();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Resets the {MD5HashGenerator} to its initial state.
        /// ----------------------------------------------------------------------------------------
        void Reset() noexcept
        {
            Md5Initialise(&m_context);
        }

        /// ----------------------------------------------------------------------------------------
        /// Adds data to the generator. This can be called infinite times.
        /// 
        /// @PARAM[IN] in_data Ptr to the input data.
        /// @PARAM[IN] in_data_size Size of the data.
        /// ----------------------------------------------------------------------------------------
        void AddData(const void* in_data, size_t in_data_size)
        {
            Md5Update(&m_context, in_data, in_data_size);
        }

        /// ----------------------------------------------------------------------------------------
        /// Generates MD5 Hash.
        /// 
        /// @PARAM[OUT] out_hash Reference to {MD5Hash} object to get output in.
        /// ----------------------------------------------------------------------------------------
        void Generate(MD5Hash& out_hash) noexcept
        {
            Md5Finalise(&m_context, reinterpret_cast<MD5_HASH*>(out_hash.bytes));
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Underlying MD5 implementation.
        /// ----------------------------------------------------------------------------------------
        Md5Context m_context;
    };

    /// --------------------------------------------------------------------------------------------
    /// Generates MD5 Hash using {MD5HashGenerator}.
    /// --------------------------------------------------------------------------------------------
    MD5Hash ATOM_API GENERATE_MD5_HASH(const void* in_data, const size_t in_size)
    {
        MD5HashGenerator generator;
        MD5Hash hash;

        generator.AddData(in_data, in_size);
        generator.Generate(hash);

        return hash;
    }
}