#pragma once

extern "C"
{
    #include "WjCryptLib_Md5.h"
}

#include "Atom/Core.h"
#include "Atom/Exceptions.h"

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
        /// @PARAM[IN] in_dataSize Size of the data.
        /// ----------------------------------------------------------------------------------------
        void ProcessBytes(const void* in_data, size_t in_dataSize)
        {
            ATOM_DEBUG_EXPECTS(in_data != nullptr);
            ATOM_DEBUG_EXPECTS(in_dataSize > 0);

            Md5Update(&m_context, in_data, in_dataSize);
        }

        /// ----------------------------------------------------------------------------------------
        /// Processes single byte. This can be called infinite times.
        /// 
        /// @PARAM[IN] in_data Data to process.
        /// @PARAM[IN] in_dataSize Size of the data.
        /// 
        /// @EXPECTS {in_data != nullptr}.
        /// @EXPECTS {in_dataSize > 0}.
        /// 
        /// @THROWS AssertionException Expects {in_data != nullptr}.
        /// @THROWS AssertionException Expects {in_dataSize > 0}.
        /// ----------------------------------------------------------------------------------------
        void ProcessByte(byte in_data)
        {
            Md5Update(&m_context, &in_data, 1);
        }

        /// ----------------------------------------------------------------------------------------
        /// Generates Md5 Hash.
        /// 
        /// @RETURNS {Md5Hash} object.
        /// ----------------------------------------------------------------------------------------
        Md5Hash Generate() noexcept
        {
            Md5Hash hash;
            Md5Finalise(&m_context, reinterpret_cast<MD5_HASH*>(hash.bytes));
            return hash;
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
    inline Md5Hash GENERATE_Md5_HASH(const void* in_data, const size_t in_dataSize)
    {
        Md5HashGenerator generator;

        generator.ProcessBytes(in_data, in_dataSize);
        return generator.Generate();
    }
}