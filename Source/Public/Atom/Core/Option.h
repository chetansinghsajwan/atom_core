#pragma once
#include "Atom/Invokable/Invokable.h"
#include "_OptionImpl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Type used to denote null state for [`Option`].
    /// --------------------------------------------------------------------------------------------
    class NullOption
    {};

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
    /// like `i32`.
    ///
    /// # Template Parameters
    /// - `T`: Type of value to store.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
        requires(not RIsVoid<T>) and (not TTI::IsQualified<T>) and (not TTI::IsRValueRef<T>)
    class Option
    {
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
        constexpr Option():
            _impl{ _ImplCtorNoVal{} }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(const Option& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        ///
        /// - If `that` contains value, copy constructs `this` value with `that` value.
        /// - Else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(const Option& that)
            requires(RCopyConstructible<T>) and (not RTriviallyCopyConstructible<T>)
        {
            _impl.constructValueFromOption(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const Option& that) -> Option&
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
        constexpr auto operator=(const Option& that) -> Option&
            requires(RCopyConstructible<T>) and (RCopyAssignable<T>)
                    and (not RTriviallyCopyConstructible<T>) and (not RTriviallyCopyAssignable<T>)
        {
            _impl.assignValueFromOption(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Option(Option&& that)
            requires(RTriviallyMoveConstructible<T>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        ///
        /// If `that` contains value, move constructs `this` value with `that` value.
        /// Else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(Option&& that)
            requires(RMoveConstructible<T>) and (not RTriviallyMoveConstructible<T>)
        {
            _impl.constructValueFromOption(mov(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(Option&& that) -> Option&
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
        constexpr auto operator=(Option&& that) -> Option&
            requires(RMoveConstructible<T>) and (RMoveAssignable<T>)
                    and (not RTriviallyMoveConstructible<T>) and (not RTriviallyMoveAssignable<T>)
        {
            _impl.assignValueFromOption(mov(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Null Constructor
        ///
        /// Constructs with no value.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(NullOption):
            _impl{ _ImplCtorNoVal{} }
        {}

        /// ----------------------------------------------------------------------------------------
        /// Null Operator.
        ///
        /// If `this` contains value, destroys it.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(NullOption) -> Option&
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
        constexpr Option(const T& val):
            _impl{ val }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Param Copy Operator
        ///
        /// - If `this` contains value, copy assigns `this` value with `val`.
        /// - Else, copy constructs `this` value with `val`.
        ///
        /// # Parameters
        /// - `val`: Value to assign or construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const T& val) -> Option&
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
        constexpr Option(T&& val):
            _impl{ mov(val) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Param Move Operator
        ///
        /// - If `this` contains value, move assigns `this` value with `val`.
        /// - Else, move constructs `this` value with `val`.
        ///
        /// # Parameters
        /// - `val`: Value to assign or construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(T&& val) -> Option&
        {
            _impl.assignValue(mov(val));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~Option()
            requires(RTriviallyDestructible<T>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        ///
        /// Destroys value if stored.
        /// ----------------------------------------------------------------------------------------
        constexpr ~Option()
            requires(RDestructible<T>) and (not RTriviallyDestructible<T>)
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
        constexpr auto emplace(TArgs&&... args)
            requires(RConstructible<T, TArgs...>)
        {
            _impl.emplaceValue(forward<TArgs>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto value() & -> T&
        {
            expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto value() const& -> const T&
        {
            expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto value() && -> T&&
        {
            expects(isValue(), "Doesn't contain value.");

            return mov(_impl.getValue());
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() & -> T&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() const& -> const T&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() && -> T&&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() -> T*
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return &_impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() const -> const T*
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return &_impl.getValue();
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
        constexpr auto valueOrInvoke(TInvokable&& other) const -> const T&
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
        constexpr auto valueOr(const T& other) const -> const T&
        {
            return valueOrInvoke([&other]() { return other; });
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `this` value or default.
        ///
        /// - If `this` contains value, get `this` value.
        /// - Else, get default constructed value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto valueOrDefault() const -> const T&
            requires(RDefaultConstructible<T>)
        {
            return valueOrInvoke([&]() { return _impl.GetDefault(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// Is `this` contains value or not.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isValue() const -> bool
        {
            return _impl.isValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroys value if stored.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reset()
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
        constexpr auto swap(Option& that)
        {
            return _impl.swapValueFromOption(that._impl);
        }

    private:
        _Impl _impl;
    };

    /// --------------------------------------------------------------------------------------------
    /// Specializaion of [`Option`] for ref types.
    ///
    /// - `T`: Type of ref to store.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class Option<T&>
    {
        using _Impl = _OptionImpl<T*>;
        using _ImplCtorNoVal = _Impl::CtorNoVal;

    public:
        using TValue = T&;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        ///
        /// Constructs with null state. [`isValue()`] will return false.
        /// ----------------------------------------------------------------------------------------
        constexpr Option():
            _impl{ _ImplCtorNoVal{} }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(const Option& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const Option& that) -> Option& = default;

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Option(Option&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(Option&& that) -> Option& = default;

        /// ----------------------------------------------------------------------------------------
        /// # Null Constructor
        ///
        /// Constructs with null state.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(NullOption):
            _impl{ _ImplCtorNoVal{} }
        {}

        /// ----------------------------------------------------------------------------------------
        /// Null Operator.
        ///
        /// Switches to null state.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(NullOption) -> Option&
        {
            _impl.destroyValueWithChecks();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Constructor
        ///
        /// Constructs with ref `ref`.
        ///
        /// # Parameters
        /// - `ref`: Ref to assign.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(T& ref):
            _impl{ &ref }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Value Operator
        ///
        /// Assigns new ref.
        ///
        /// # Parameters
        /// - `ref`: Ref to assign.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(T& ref) -> Option&
        {
            _impl.assignValue(&ref);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~Option() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Assigns new ref.
        ///
        /// # Parameters
        /// - `ref`: Ref to assign.
        /// ----------------------------------------------------------------------------------------
        constexpr auto emplace(T& ref)
        {
            _impl.emplaceValue(ref);
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto value() & -> T&
        {
            expects(isValue(), "Doesn't contain value.");

            return *_impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto value() const& -> const T&
        {
            expects(isValue(), "Doesn't contain value.");

            return *_impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto value() && -> T&&
        {
            expects(isValue(), "Doesn't contain value.");

            return mov(*_impl.getValue());
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() & -> T&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return *_impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() const& -> const T&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return *_impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() && -> T&&
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return *_impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() -> T*
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() const -> const T*
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// # To Do: Update this.
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        constexpr auto valueOrInvoke(TInvokable&& other) const -> const T&
            requires RInvokable<TInvokable, const T&()>
        {
            if (not _impl.isValue())
            {
                return other();
            }

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// # To Do: Update this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto valueOr(const T& other) const -> const T&
        {
            return valueOrInvoke([&other]() { return other; });
        }

        /// ----------------------------------------------------------------------------------------
        /// # To Do: Update this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto valueOrDefault() const -> const T&
            requires(RDefaultConstructible<T>)
        {
            return valueOrInvoke([&]() { return _impl.GetDefault(); });
        }

        /// ----------------------------------------------------------------------------------------
        /// Is `this` contains ref or not.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isValue() const -> bool
        {
            return _impl.isValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Switches to null state.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reset()
        {
            return _impl.destroyValueWithCheck();
        }

        /// ----------------------------------------------------------------------------------------
        /// Swap refs and state with `that`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto swap(Option& that)
        {
            return _impl.swapValueFromOption(that._impl);
        }

    private:
        _Impl _impl;
    };

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// Compairision Operators for [`Option`] with [`nullopt`].
////
////////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// # Equality Comparision Operator
    ///
    /// Returns `true` if `opt` is null.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    constexpr auto operator==(const Option<T>& opt, NullOption) -> bool
    {
        return not opt.isValue();
    }

    /// --------------------------------------------------------------------------------------------
    /// # Not Equality Comparision Operator
    ///
    /// Performs negation of [Equality Comparision Operator].
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    constexpr auto operator!=(const Option<T>& opt, NullOption) -> bool
    {
        return not(opt == nullopt);
    }

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// Compairision Operators for [`Option`] with [`Option`].
////
////////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// # Equality Comparision Operator
    ///
    /// - If `opt0` and `opt1` are null, returns `true`.
    /// - If `opt0` is null and `opt1` is not null or vice versa, returns `false`.
    /// - If `opt0` and `opt1` are not null, returns `*opt0 == *opt1`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr auto operator==(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(REqualityComparableWith<T0, T1>)
    {
        if (opt0.isValue() != opt1.isValue())
            // One is null and one has value.
            return false;

        if (opt0 == nullopt)
            // Both are null.
            return true;

        return *opt0 == *opt1;
    }

    /// --------------------------------------------------------------------------------------------
    /// # Not Equality Comparision Operator
    ///
    /// Performs negation of [Equality Comparision Operator].
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr auto operator!=(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(REqualityComparableWith<T0, T1>)
    {
        return not(opt0 == opt1);
    }

    /// --------------------------------------------------------------------------------------------
    /// # Less Than Comparision Operator
    ///
    /// - If `opt0` or `opt1` is null, returns false.
    /// - Else, returns `*opt0 < *opt1`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr auto operator<(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(RComparableWith<T0, T1>)
    {
        if (opt0 == nullopt or opt1 == nullopt)
            return false;

        return *opt0 < *opt1;
    }

    /// --------------------------------------------------------------------------------------------
    /// # Greater Than Comparision Operator
    ///
    /// - If `opt0` or `opt1` is null, returns false.
    /// - Else, returns `*opt0 > *opt1`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr auto operator>(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(RComparableWith<T0, T1>)
    {
        if (opt0 == nullopt or opt1 == nullopt)
            return false;

        return *opt0 > *opt1;
    }

    /// --------------------------------------------------------------------------------------------
    /// # Less Than or Equal To Comparision Operator
    ///
    /// - If `opt0` or `opt1` is null, returns false.
    /// - Else, returns `*opt0 <= *opt1`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr auto operator<=(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(RComparableWith<T0, T1>)
    {
        if (opt0 == nullopt or opt1 == nullopt)
            return false;

        return *opt0 <= *opt1;
    }

    /// --------------------------------------------------------------------------------------------
    /// # Greater Than or Equal To Comparision Operator
    ///
    /// - If `opt0` or `opt1` is null, returns false.
    /// - Else, returns `*opt0 >= *opt1`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    constexpr auto operator>=(const Option<T0>& opt0, const Option<T1>& opt1) -> bool
        requires(RComparableWith<T0, T1>)
    {
        if (opt0 == nullopt or opt1 == nullopt)
            return false;

        return *opt0 >= *opt1;
    }
}
