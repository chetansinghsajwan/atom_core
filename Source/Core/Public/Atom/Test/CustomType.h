#pragma once
#include "Atom/Core.h"

namespace Atom::Test
{
    enum class ECustomTypeFlags: uint64
    {
        None = 0,
        All = uint64(-1),

        DefaultConstructor                      = 1 << 0,
        CopyConstructor                         = 1 << 1,
        CopyOperator                            = 1 << 2,
        MoveConstructor                         = 1 << 3,
        MoveOperator                            = 1 << 4,
        Destructor                              = 1 << 5,

        TrivialDefaultConstructor               = 1 << 6,
        TrivialCopyConstructor                  = 1 << 7,
        TrivialCopyOperator                     = 1 << 8,
        TrivialMoveConstructor                  = 1 << 9,
        TrivialMoveOperator                     = 1 << 10,
        TrivialDestructor                       = 1 << 11,

        Trivial                                 = 1 << 12,

        EqualToOperator                         = 1 << 13,
        NotEqualToOperator                      = 1 << 14,
        LessThanToOperator                      = 1 << 15,
        GreaterThanToOperator                   = 1 << 16,
        LessThanOrEqualToOperator               = 1 << 17,
        GreaterThanOrEqualToOperator            = 1 << 18,
    };

    class EnumFlagUtils
    {
    public:
        template <typename TEnum>
        static constexpr fn HasFlag(const TEnum& e, const TEnum& flag) -> bool
        {
            return (e & flag) == flag;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <ECustomTypeFlags flags>
    class CustomType
    {
        /// ----------------------------------------------------------------------------------------
        /// # Trivial Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CustomType()
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialDefaultConstructor))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CustomType()
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::DefaultConstructor))
                and (not EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialDefaultConstructor))
        { }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CustomType(const CustomType& that)
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialCopyConstructor))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CustomType(const CustomType& that)
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::CopyConstructor))
                and (not EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialCopyConstructor))
        { }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(const CustomType& that) -> CustomType&
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialCopyOperator))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(const CustomType& that) -> CustomType&
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::CopyOperator))
                and (not EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialCopyOperator))
        {
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CustomType(CustomType&& that)
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialMoveConstructor))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr CustomType(CustomType&& that)
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::MoveConstructor))
                and (not EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialMoveConstructor))
        { }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(CustomType&& that) -> CustomType&
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialMoveOperator))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(CustomType&& that) -> CustomType&
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::MoveOperator))
                and (not EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialMoveOperator))
        {
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr CustomType(CustomType&& that)
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialDestructor))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr CustomType(CustomType&& that)
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::Destructor))
                and (not EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::TrivialDestructor))
        { }

    //// -------------------------------------------------------------------------------------------
    //// Compairision Operators
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        /// # Equal To Opeartor
        /// ----------------------------------------------------------------------------------------
        constexpr fn op==(const CustomType& that) -> bool
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::EqualToOperator))
        {
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Not Equal To Opeartor
        /// ----------------------------------------------------------------------------------------
        constexpr fn op!=(const CustomType& that) -> bool
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::NotEqualToOperator))
        {
            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Less Than Opeartor
        /// ----------------------------------------------------------------------------------------
        constexpr fn op<(const CustomType& that) -> bool
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::LessThanToOperator))
        {
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Greater Than Opeartor
        /// ----------------------------------------------------------------------------------------
        constexpr fn op>(const CustomType& that) -> bool
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::GreaterThanToOperator))
        {
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Less Than Or Equal To Opeartor
        /// ----------------------------------------------------------------------------------------
        constexpr fn op<=(const CustomType& that) -> bool
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::LessThanOrEqualToOperator))
        {
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Greater Than Or Equal To Opeartor
        /// ----------------------------------------------------------------------------------------
        constexpr fn op>=(const CustomType& that) -> bool
            requires(EnumFlagUtils::HasFlag(flags, ECustomTypeFlags::GreaterThanOrEqualToOperator))
        {
            return true;
        }
    };
}