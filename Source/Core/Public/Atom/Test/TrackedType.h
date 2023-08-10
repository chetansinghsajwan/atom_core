#pragma once
#include "Atom/Core.h"

namespace Atom::Test
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

        constexpr fn operator=(const TrackedType& that) -> TrackedType&
        {
            lastOp = EOperation::CopyOperator;
            that.lastOp = EOperation::CopyOperatorAsThat;
            return self;
        }

        constexpr TrackedType(TrackedType&& that)
        {
            lastOp = EOperation::MoveConstructor;
            that.lastOp = EOperation::MoveConstructorAsThat;
        }

        constexpr fn operator=(TrackedType&& that) -> TrackedType&
        {
            lastOp = EOperation::MoveOperator;
            that.lastOp = EOperation::MoveOperatorAsThat;
            return self;
        }

        constexpr ~TrackedType()
        {
            lastOp = EOperation::Destructor;
        }

    public:
        constexpr fn operator==(const TrackedType& that) const -> bool
        {
            lastOp = EOperation::EqualOperator;
            that.lastOp = EOperation::EqualOperator;
            return true;
        }

        constexpr fn operator!=(const TrackedType& that) const -> bool
        {
            return not (self == that);
        }

        constexpr fn operator<(const TrackedType& that) const -> bool
        {
            lastOp = EOperation::LessThanOperator;
            that.lastOp = EOperation::LessThanOperator;
            return true;
        }

        constexpr fn operator>(const TrackedType& that) const -> bool
        {
            lastOp = EOperation::GreaterThanOperator;
            that.lastOp = EOperation::GreaterThanOperator;
            return true;
        }

        constexpr fn operator<=(const TrackedType& that) const -> bool
        {
            lastOp = EOperation::LessThanOrEqualOperator;
            that.lastOp = EOperation::LessThanOrEqualOperator;
            return true;
        }

        constexpr fn operator>=(const TrackedType& that) const -> bool
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
    class TrackedTypeOf : public TrackedType
    {
        using Base = TrackedType;

    public:
        using Base::Base;
        using Base::operator=;

    public:
        T value;
    };
}