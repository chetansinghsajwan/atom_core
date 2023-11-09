#pragma once
#include "Atom/Core.h"
#include "catch2/catch_tostring.hpp"

namespace Atom::Tests
{
    /// --------------------------------------------------------------------------------------------
    /// Type used to track object state.
    /// --------------------------------------------------------------------------------------------
    class TrackedType
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Enum used to represent state.
        /// ----------------------------------------------------------------------------------------
        enum class EOperation
        {
            None,

            DefaultConstructor,
            CopyConstructor,
            CopyConstructorAsThat,
            CopyOperator,
            CopyOperatorAsThat,
            MoveConstructor,
            MoveConstructorAsThat,
            MoveOperator,
            MoveOperatorAsThat,
            Destructor,

            EqualOperator,
            LessThanOperator,
            GreaterThanOperator,
            LessThanOrEqualOperator,
            GreaterThanOrEqualOperator
        };

        static constexpr auto ToStringView(EOperation op) -> std::string_view
        {
            switch (op)
            {
                case EOperation::None:                       return "None";
                case EOperation::DefaultConstructor:         return "DefaultConstructor";
                case EOperation::CopyConstructor:            return "CopyConstructor";
                case EOperation::CopyConstructorAsThat:      return "CopyConstructorAsThat";
                case EOperation::CopyOperator:               return "CopyOperator";
                case EOperation::CopyOperatorAsThat:         return "CopyOperatorAsThat";
                case EOperation::MoveConstructor:            return "MoveConstructor";
                case EOperation::MoveConstructorAsThat:      return "MoveConstructorAsThat";
                case EOperation::MoveOperator:               return "MoveOperator";
                case EOperation::MoveOperatorAsThat:         return "MoveOperatorAsThat";
                case EOperation::Destructor:                 return "Destructor";
                case EOperation::EqualOperator:              return "EqualOperator";
                case EOperation::LessThanOperator:           return "LessThanOperator";
                case EOperation::GreaterThanOperator:        return "GreaterThanOperator";
                case EOperation::LessThanOrEqualOperator:    return "LessThanOrEqualOperator";
                case EOperation::GreaterThanOrEqualOperator: return "GreaterThanOrEqualOperator";
                default:                                     return "UNKNOWN";
            }
        }

    public:
        constexpr TrackedType()
        {
            lastOp = EOperation::DefaultConstructor;
        }

        constexpr TrackedType(const TrackedType& that)
        {
            lastOp = EOperation::CopyConstructor;
            that.lastOp = EOperation::CopyConstructorAsThat;
        }

        constexpr auto operator=(const TrackedType& that) -> TrackedType&
        {
            lastOp = EOperation::CopyOperator;
            that.lastOp = EOperation::CopyOperatorAsThat;
            return *this;
        }

        constexpr TrackedType(TrackedType&& that)
        {
            lastOp = EOperation::MoveConstructor;
            that.lastOp = EOperation::MoveConstructorAsThat;
        }

        constexpr auto operator=(TrackedType&& that) -> TrackedType&
        {
            lastOp = EOperation::MoveOperator;
            that.lastOp = EOperation::MoveOperatorAsThat;
            return *this;
        }

        constexpr ~TrackedType()
        {
            lastOp = EOperation::Destructor;
        }

    public:
        constexpr auto eq(const TrackedType& that) const -> bool
        {
            lastOp = EOperation::EqualOperator;
            that.lastOp = EOperation::EqualOperator;
            return true;
        }

        constexpr auto ne(const TrackedType& that) const -> bool
        {
            return not(*this == that);
        }

        constexpr auto lt(const TrackedType& that) const -> bool
        {
            lastOp = EOperation::LessThanOperator;
            that.lastOp = EOperation::LessThanOperator;
            return true;
        }

        constexpr auto gt(const TrackedType& that) const -> bool
        {
            lastOp = EOperation::GreaterThanOperator;
            that.lastOp = EOperation::GreaterThanOperator;
            return true;
        }

        constexpr auto le(const TrackedType& that) const -> bool
        {
            lastOp = EOperation::LessThanOrEqualOperator;
            that.lastOp = EOperation::LessThanOrEqualOperator;
            return true;
        }

        constexpr auto ge(const TrackedType& that) const -> bool
        {
            lastOp = EOperation::GreaterThanOrEqualOperator;
            that.lastOp = EOperation::GreaterThanOrEqualOperator;
            return true;
        }

    public:
        mutable EOperation lastOp;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class TrackedTypeOf: public TrackedType
    {
        using Base = TrackedType;

    public:
        using Base::Base;
        using Base::operator=;

    public:
        T value;
    };
}

namespace Catch
{
    template <>
    class StringMaker<Atom::Tests::TrackedType::EOperation>
    {
    public:
        static constexpr auto convert(Atom::Tests::TrackedType::EOperation op) -> std::string
        {
            return std::string(Atom::Tests::TrackedType::ToStringView(op));
        }
    };
}
