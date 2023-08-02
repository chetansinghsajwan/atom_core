#pragma once
#include "_OptionImpl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Type used to denote null state for [`Option`].
    /// --------------------------------------------------------------------------------------------
    class NullOption { };

    /// --------------------------------------------------------------------------------------------
    /// Used as keyword to with [`Option`].
    /// --------------------------------------------------------------------------------------------
    constexpr inline auto nullopt = NullOption{};

    /// --------------------------------------------------------------------------------------------
    /// The Option class is used to wrap the object of type `T`. This class contain either the 
    /// value or can be empty representing no value.
    /// 
    /// This is useful when we want to return a value that may or may not exist, without 
    /// using null pointers or exceptions. Or just want to add the ability of being null to a type 
    /// like `int`.
    ///
    /// # Template Parameters
    /// - `T`: Type of value to store.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class Option
    {
        template <typename T1>
        friend class Option;

        using _Impl = _OptionImpl<T>;
        using _ImplCtorNoVal = _Impl::CtorNoVal;

    public:
        using TValue = T;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// 
        /// Constructs with no value. [`isValue()`] will return false.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor Option():
            _impl{ _ImplCtorNoVal{} } { }

        /// ----------------------------------------------------------------------------------------
        /// Trivial Copy Constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor Option(const Option& that)
            requires(RTriviallyCopyConstructible<T>)
            = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// 
        /// - If `that` contains value, copy constructs `this` value with `that` value.
        /// - Else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor Option(const Option& that)
            requires(RCopyConstructible<T>)
                and (not RTriviallyCopyConstructible<T>)
        {
            _impl.constructValueFromOption(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(const Option& that) -> Option&
            requires(RTriviallyCopyAssignable<T>)
            = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// 
        /// - If `that` contains value
        ///     - If `this` contains value, copy assigns with `that` value.
        ///     - Else, copy constructs with `that` value.
        /// - Else
        ///     - If `this` contains value, destroys `this` value.
        ///     - Else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(const Option& that) -> Option&
            requires(RCopyConstructible<T>)
                and (RCopyAssignable<T>)
                and (not RTriviallyCopyConstructible<T>)
                and (not RTriviallyCopyAssignable<T>)
        {
            _impl.assignValueFromOption(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr ctor Option(Option&& that)
            requires(RTriviallyMoveConstructible<T>)
            = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// 
        /// If `that` contains value, move constructs `this` value with `that` value.
        /// Else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor Option(Option&& that)
            requires(RMoveConstructible<T>)
                and (not RTriviallyMoveConstructible<T>)
        {
            _impl.constructValueFromOption(mov(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(Option&& that) -> Option&
            requires(RTriviallyMoveAssignable<T>)
            = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// 
        /// - If `that` contains value
        ///     - If `this` contains value, move assigns with `that` value.
        ///     - Else, move constructs with `that` value.
        /// - Else
        ///     - If `this` contains value, destroys `this` value.
        ///     - Else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(Option&& that) -> Option&
            requires(RMoveConstructible<T>)
                and (RMoveAssignable<T>)
                and (not RTriviallyMoveConstructible<T>)
                and (not RTriviallyMoveAssignable<T>)
        {
            _impl.assignValueFromOption(mov(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Null Constructor
        /// 
        /// Constructs with no value.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor Option(NullOption):
            _impl{ _ImplCtorNoVal{} } { }

        /// ----------------------------------------------------------------------------------------
        /// Null Operator.
        /// 
        /// If `this` contains value, destroys it.
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(NullOption) -> Option&
        {
            _impl.destroyValueWithChecks();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Param Copy Constructor
        /// 
        /// Copy constructs `this` value with `val`.
        /// 
        /// # Parameters
        /// - `val`: Value to construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor Option(const T& val):
            _impl{ val } { }

        /// ----------------------------------------------------------------------------------------
        /// # Param Copy Operator
        /// 
        /// - If `this` contains value, copy assigns `this` value with `val`.
        /// - Else, copy constructs `this` value with `val`.
        /// 
        /// # Parameters
        /// - `val`: Value to assign or construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(const T& val) -> Option&
        {
            _impl.assignValue(val);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Param Move Constructor
        /// 
        /// Move constructs `this` value with `value`.
        /// 
        /// # Parameters
        /// - `val`: Value to construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor Option(T&& val):
            _impl{ mov(val) } { }

        /// ----------------------------------------------------------------------------------------
        /// # Param Move Operator
        /// 
        /// - If `this` contains value, move assigns `this` value with `val`.
        /// - Else, move constructs `this` value with `val`.
        /// 
        /// # Parameters
        /// - `val`: Value to assign or construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr fn op=(T&& val) -> Option&
        {
            _impl.assignValue(mov(val));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr dtor Option()
            requires(RTriviallyDestructible<T>)
            = default;

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// 
        /// Destroys value if stored.
        /// ----------------------------------------------------------------------------------------
        constexpr dtor Option()
            requires(RDestructible<T>)
                and (not RTriviallyDestructible<T>)
        {
            _impl.destroyValueOnDestructor();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Construct value with `args`. If a value already exists, destroys that value and 
        /// contructs and new one.
        /// 
        /// # Parameters
        /// - `args`: Arguments to construct the new value with.
        /// ----------------------------------------------------------------------------------------
        template <typename... TArgs>
        constexpr fn emplace(TArgs&&... args)
            requires(RConstructible<T, TArgs...>)
        {
            _impl.emplaceValue(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value.
        /// 
        /// # Returns
        /// L-Value reference to the value stored.
        /// ----------------------------------------------------------------------------------------
        constexpr fn value() & -> T&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value.
        /// 
        /// # Returns
        /// Const l-value reference to the value stored.
        /// ----------------------------------------------------------------------------------------
        constexpr fn value() const& -> const T&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value.
        /// 
        /// # Returns
        /// R-Value reference to the value stored.
        /// ----------------------------------------------------------------------------------------
        constexpr fn value() && -> T&&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return mov(_impl.getValue());
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls [`value()`].
        /// ----------------------------------------------------------------------------------------
        constexpr fn op*() & -> T&
        {
            return value();
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls [`value()`].
        /// ----------------------------------------------------------------------------------------
        constexpr fn op*() const& -> const T&
        {
            return value();
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls [`value()`].
        /// ----------------------------------------------------------------------------------------
        constexpr fn op*() && -> T&&
        {
            return value();
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls [`&value()`].
        /// ----------------------------------------------------------------------------------------
        constexpr fn op->() -> T*
        {
            return &value();
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls [`&value()`].
        /// ----------------------------------------------------------------------------------------
        constexpr fn op->() const -> const T*
        {
            return &value();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `this` value or.
        /// 
        /// - If `this` contains value, get `this` value.
        /// - Else, get value returned by invokable `other`.
        /// 
        /// # Parameters
        /// - `other`: Invokable to return other value.
        /// 
        /// # Returns
        /// Const reference to `this` value or other value returned by invokable `other`.
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        constexpr fn valueOrInvoke(TInvokable&& other) const -> const T&
            requires RInvokable<TInvokable, const T&()>
        {
            if (not _impl.isValue())
            {
                return other();
            }

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `this` value or.
        /// 
        /// - If `this` contains value, get `this` value.
        /// - Else, get value `other`.
        /// 
        /// # Parameters
        /// - `other`: Other value to return.
        /// 
        /// # Returns
        /// Const reference to `this` value or `other`.
        /// ----------------------------------------------------------------------------------------
        constexpr fn valueOr(const T& other) const -> const T&
        {
            return valueOrInvoke([&other]()
                {
                    return other;
                });
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `this` value or default.
        /// 
        /// - If `this` contains value, get `this` value.
        /// - Else, get default constructed value.
        /// ----------------------------------------------------------------------------------------
        constexpr fn valueOrDefault() const -> const T&
            requires(RDefaultConstructible<T>)
        {
            return valueOrInvoke([&]()
                {
                    return _impl.GetDefault();
                });
        }

        /// ----------------------------------------------------------------------------------------
        /// Is `this` contains value or not.
        /// ----------------------------------------------------------------------------------------
        constexpr fn isValue() const -> bool
        {
            return _impl.isValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroys value if stored.
        /// ----------------------------------------------------------------------------------------
        constexpr fn reset()
        {
            return _impl.destroyValueWithCheck();
        }

        /// ----------------------------------------------------------------------------------------
        /// Swap values and state with `that`.
        /// 
        /// - If `that` contains value
        ///     - If `this` contains value, swaps `this` value with `that` value.
        ///     - Else, move constructs `this` value with `that` value.
        /// - Else
        ///     - If `this` contains value, move constructs `that` value with `this` value.
        ///     - Else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr fn swap(Option& that)
        {
            return _impl.swapValueFromOption(that._impl);
        }

    private:
        _Impl _impl;
    };

    /// --------------------------------------------------------------------------------------------
    /// # To Do: Review this case.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class Option<T&> extends Option<T*>
    {
        using Base = Option<T*>;

    public:
        constexpr ctor Option(T& ref):
            Base{ &ref } { }

        using Base::Base;
        using Base::op=;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn value() & -> T&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return *_impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn value() const& -> const T&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return *_impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn value() && -> T&&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return mov(*_impl.getValue());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn op*() & -> T&
        {
            return value();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn op*() const& -> const T&
        {
            return value();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn op*() && -> T&&
        {
            return value();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn op->() -> T*
        {
            return value();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn op->() const -> const T*
        {
            return value();
        }

        using Base::isValue;
    
    private:
        using Base::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    /// # To Do: Review this case.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class Option<T&&>
    {
        static_assert(sizeof(T) == -1, "rvalue reference types are not supported.");
    };

//// -----------------------------------------------------------------------------------------------
//// Compairision Operators for [`Option`] with [`Option`].
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// # Equality Comparision Operator
    /// 
    /// - If `opt0` and `opt1` are null, returns `true`.
    /// - If `opt0` is null and `opt1` is not null or vice versa, returns `false`.
    /// - If `opt0` and `opt1` are not null, returns `opt0.value() == opt1.value()`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr fn op==(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(REqualityComparableWith<T0, T1>)
    {
        if (opt0.isValue() != opt1.isValue())
            // One is null and one has value.
            return false;

        if (not opt0.isValue())
            // Both are null.
            return true;

        return opt0.value() == opt1.value();
    }

    /// --------------------------------------------------------------------------------------------
    /// # Not Equality Comparision Operator
    /// 
    /// Performs negation of [Equality Comparision Operator].
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr fn op!=(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(REqualityComparableWith<T0, T1>)
    {
        return not (opt0 == opt1);
    }

    /// --------------------------------------------------------------------------------------------
    /// # Less Than Comparision Operator
    /// 
    /// - If `opt0` or `opt1` is null, returns false.
    /// - Else, returns `opt0.value() < opt1.value()`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr fn op<(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(RComparableWith<T0, T1>)
    {
        if (not opt0.isValue() or not opt1.isValue())
            return false;

        return opt0.value() < opt1.value();
    }

    /// --------------------------------------------------------------------------------------------
    /// # Greater Than Comparision Operator
    /// 
    /// - If `opt0` or `opt1` is null, returns false.
    /// - Else, returns `opt0.value() > opt1.value()`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr fn op>(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(RComparableWith<T0, T1>)
    {
        if (not opt0.isValue() or not opt1.isValue())
            return false;

        return opt0.value() > opt1.value();
    }

    /// --------------------------------------------------------------------------------------------
    /// # Less Than or Equal To Comparision Operator
    /// 
    /// - If `opt0` or `opt1` is null, returns false.
    /// - Else, returns `opt0.value() <= opt1.value()`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr fn op<=(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(RComparableWith<T0, T1>)
    {
        if (not opt0.isValue() or not opt1.isValue())
            return false;

        return opt0.value() <= opt1.value();
    }

    /// --------------------------------------------------------------------------------------------
    /// # Greater Than or Equal To Comparision Operator
    /// 
    /// - If `opt0` or `opt1` is null, returns false.
    /// - Else, returns `opt0.value() >= opt1.value()`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr fn op>=(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(RComparableWith<T0, T1>)
    {
        if (not opt0.isValue() or not opt1.isValue())
            return false;

        return opt0.value() >= opt1.value();
    }

//// -----------------------------------------------------------------------------------------------
//// Compairision Operators for [`Option`] with [`nullopt`].
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// # Equality Comparision Operator
    /// 
    /// Returns `true` if `opt` is null.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    constexpr fn op==(const Option<T>& opt, NullOption) -> bool
    {
        return not opt.isValue();
    }

    /// --------------------------------------------------------------------------------------------
    /// # Not Equality Comparision Operator
    /// 
    /// Performs negation of [Equality Comparision Operator].
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    constexpr fn op!=(const Option<T>& opt, NullOption) -> bool
    {
        return not (opt0 == opt1);
    }
}