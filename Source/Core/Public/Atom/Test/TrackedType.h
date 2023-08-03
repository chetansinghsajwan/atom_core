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
            NotEqualOperator,
            LessThanOperator,
            GreaterThanOperator,
            LessThanOrEqualOperator,
            GreaterThanOrEqualOperator
        };

    public:
        constexpr ctor TrackedType()
        {
            lastOp = EOperation::DefaultConstructor;
        }

        constexpr ctor TrackedType(const TrackedType& that)
        {
            lastOp = EOperation::CopyConstructor;
            that.lastOp = EOperation::CopyConstructorAsThat;
        }

        constexpr fn op=(const TrackedType& that) -> TrackedType&
        {
            lastOp = EOperation::CopyOperator;
            that.lastOp = EOperation::CopyOperatorAsThat;
            return *this;
        }

        constexpr ctor TrackedType(TrackedType&& that)
        {
            lastOp = EOperation::MoveConstructor;
            that.lastOp = EOperation::MoveConstructorAsThat;
        }

        constexpr fn op=(TrackedType&& that) -> TrackedType&
        {
            lastOp = EOperation::MoveOperator;
            that.lastOp = EOperation::MoveOperatorAsThat;
            return *this;
        }

        constexpr dtor TrackedType()
        {
            lastOp = EOperation::Destructor;
        }

    public:
        constexpr fn op==(const TrackedType& opt1) const -> bool
        {
            lastOp = EOperation::EqualOperator;
            return true;
        }

        constexpr fn op!=(const TrackedType& opt1) const -> bool
        {
            lastOp = EOperation::NotEqualOperator;
            return false;
        }

        constexpr fn op<(const TrackedType& opt1) const -> bool
        {
            lastOp = EOperation::LessThanOperator;
            return true;
        }

        constexpr fn op>(const TrackedType& opt1) const -> bool
        {
            lastOp = EOperation::GreaterThanOperator;
            return true;
        }

        constexpr fn op<=(const TrackedType& opt1) const -> bool
        {
            lastOp = EOperation::LessThanOrEqualOperator;
            return true;
        }

        constexpr fn op>=(const TrackedType& opt1) const -> bool
        {
            lastOp = EOperation::GreaterThanOrEqualOperator;
            return true;
        }

    public:
        mutable EOperation lastOp;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class TrackedTypeOf extends TrackedType
    {
        using Base = TrackedType;

    public:
        Base::Base;
        Base::op=;

    public:
        T value;
    };
}