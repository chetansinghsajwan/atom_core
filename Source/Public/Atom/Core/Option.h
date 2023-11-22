#pragma once
#include "Atom/Invokable/Invokable.h"
#include "_OptionImpl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Type used to initialize Option with no value.
    /// --------------------------------------------------------------------------------------------
    class NullOption
    {};

    constexpr NullOption nullopt = NullOption();

    /// --------------------------------------------------------------------------------------------
    /// The Option class is used to wrap the object of type `TVal`. This class contain either the
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
    class Option
    {
        static_assert(TTI::IsPure<T>, "Option supports only pure types");
        static_assert(not TTI::IsVoid<T>, "Option doesn't support void type.");

    private:
        using This = Option<T>;
        using Impl = _OptionImpl<T>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Type of value this option holds.
        /// ----------------------------------------------------------------------------------------
        using TVal = T;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        ///
        /// Constructs with no value.
        /// ----------------------------------------------------------------------------------------
        constexpr Option()
            : _impl(typename Impl::CtorDefault())
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(const Option& that)
            requires(RTriviallyCopyConstructible<TVal>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        ///
        /// If `that` contains value, copy constructs `this` value with `that` value.
        /// Else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(const Option& that)
            requires(not RTriviallyCopyConstructible<TVal>) and (RCopyConstructible<TVal>)
            : _impl(typename Impl::CtorCopy(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const Option& that) -> Option&
            requires(RTriviallyCopyAssignable<TVal>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        ///
        /// If `that` contains value
        ///     If `this` contains value, copy assigns with `that` value.
        ///     Else, copy constructs with `that` value.
        /// Else
        ///     If `this` contains value, destroys `this` value.
        ///     Else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const Option& that) -> Option&
            requires(not RTriviallyCopyAssignable<TVal>) and (RCopyable<TVal>)
        {
            _impl.copy(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Option(Option&& that)
            requires(RTriviallyMoveConstructible<TVal>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        ///
        /// If `that` contains value, move constructs `this` value with `that` value.
        /// Else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(Option&& that)
            requires(not RTriviallyMoveConstructible<TVal>) and (RMoveConstructible<TVal>)
            : _impl(typename Impl::CtorMove(), mov(that._impl))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(Option&& that) -> Option&
            requires(RTriviallyMoveAssignable<TVal>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        ///
        /// If `that` contains value
        ///     If `this` contains value, move assigns with `that` value.
        ///     Else, move constructs with `that` value.
        /// Else
        ///     If `this` contains value, destroys `this` value.
        ///     Else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(Option&& that) -> Option&
            requires(not RTriviallyMoveAssignable<TVal>) and (RMoveable<TVal>)
        {
            _impl.move(mov(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Null Constructor
        ///
        /// Constructs with no value.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(NullOption)
            : _impl(typename Impl::CtorDefault())
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Null Operator
        ///
        /// Destroys current value if any.
        /// ----------------------------------------------------------------------------------------
        constexpr Option& operator=(NullOption)
        {
            _impl.destroyValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Copy Constructor
        ///
        /// Copy constructs `this` value with `val`.
        ///
        /// # Parameters
        ///
        /// - `val`: Value to construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(const TVal& val)
            : _impl(val)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Value Copy Operator
        ///
        /// If `this` contains value, copy assigns `this` value with `val`.
        /// Else, copy constructs `this` value with `val`.
        ///
        /// # Parameters
        ///
        /// - `val`: Value to assign or construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const TVal& val) -> Option&
        {
            _impl.setValue(val);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Move Constructor
        ///
        /// Move constructs `this` value with `value`.
        ///
        /// # Parameters
        ///
        /// - `val`: Value to construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(TVal&& val)
            : _impl(mov(val))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Value Move Operator
        ///
        /// If `this` contains value, move assigns `this` value with `val`.
        /// Else, move constructs `this` value with `val`.
        ///
        /// # Parameters
        ///
        /// - `val`: Value to assign or construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(TVal&& val) -> Option&
        {
            _impl.setValue(mov(val));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~Option()
            requires(RTriviallyDestructible<TVal>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        ///
        /// Destroys value if stored.
        /// ----------------------------------------------------------------------------------------
        constexpr ~Option()
            requires(not RTriviallyDestructible<TVal>) and (RDestructible<TVal>)
        {
            _impl.destroy();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Construct value with `args`. If a value already exists, destroys that value and
        /// contructs and new one.
        ///
        /// # Parameters
        ///
        /// - `args`: Arguments to construct the new value with.
        /// ----------------------------------------------------------------------------------------
        template <typename... TArgs>
        constexpr auto emplace(TArgs&&... args)
            requires(RConstructible<TVal, TArgs...>)
        {
            _impl.emplaceValue(forward<TArgs>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroys current value if any.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reset()
        {
            return _impl.destroyValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get() const& -> const TVal&
        {
            Contracts::Expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getMut() & -> TVal&
        {
            Contracts::Expects(isValue(), "Doesn't contain value.");

            return _impl.getMutValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() const -> const TVal*
        {
            Contracts::DebugExpects(isValue(), "Doesn't contain value.");

            return &_impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() -> TVal*
        {
            Contracts::DebugExpects(isValue(), "Doesn't contain value.");

            return &_impl.getMutValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `this` value or `orVal`.
        ///
        /// If `this` contains value, get `this` value.
        /// Else, get value `orVal`.
        ///
        /// # Parameters
        ///
        /// - `orVal`: Other value to return.
        ///
        /// # Returns
        ///
        /// Const ref to `this` value or `orVal`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getOr(const TVal& orVal) const -> const TVal&
        {
            if (_impl.isNull())
            {
                return orVal;
            }

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `this` value or `orVal`.
        ///
        /// If `this` contains value, get `this` value.
        /// Else, get value `orVal`.
        ///
        /// # Parameters
        ///
        /// - `orVal`: Other value to return.
        ///
        /// # Returns
        ///
        /// Ref to `this` value or `orVal`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getMutOr(TVal& orVal) -> TVal&
        {
            if (_impl.isNull())
            {
                return orVal;
            }

            return _impl.getMutValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `this` value or.
        ///
        /// If `this` contains value, get `this` value.
        /// Else, get value returned by invoking `orInvoke`.
        ///
        /// # Parameters
        ///
        /// - `orInvoke`: Invokable to return orInvoke value.
        ///
        /// # Returns
        ///
        /// Const ref to `this` value or orInvoke value returned by invoking `orInvoke`.
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        constexpr auto getOrInvoke(TInvokable&& orInvoke) const -> const TVal&
            requires RInvokable<TPure<TInvokable>, const TVal&()>
        {
            if (_impl.isNull())
            {
                return orInvoke();
            }

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `this` value or.
        ///
        /// If `this` contains value, get `this` value.
        /// Else, get value returned by invoking `orInvoke`.
        ///
        /// # Parameters
        ///
        /// - `orInvoke`: Invokable to return orInvoke value.
        ///
        /// # Returns
        ///
        /// Ref to `this` value or orInvoke value returned by invoking `orInvoke`.
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        constexpr auto getMutOrInvoke(TInvokable&& orInvoke) -> TVal&
            requires RInvokable<TPure<TInvokable>, TVal&()>
        {
            if (_impl.isNull())
            {
                return orInvoke();
            }

            return _impl.getMutValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `this` value or default.
        ///
        /// If `this` contains value, get `this` value.
        /// Else, get default constructed value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getOrDefault() const& -> const TVal&
            requires(RDefaultConstructible<TVal>)
        {
            if (_impl.isNull())
            {
                return Impl::GetDefault();
            }

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Is `this` contains value or not.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isValue() const -> bool
        {
            return _impl.isValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Is `this` contains value or not.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isNull() const -> bool
        {
            return not _impl.isValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Swap values and state with `that`.
        ///
        /// If `that` contains value
        ///     If `this` contains value, swaps `this` value with `that` value.
        ///     Else, move constructs `this` value with `that` value.
        /// Else
        ///     If `this` contains value, move constructs `that` value with `this` value.
        ///     Else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto swap(Option& that)
        {
            return _impl.swap(that._impl);
        }

        /// --------------------------------------------------------------------------------------------
        /// # Equality Comparision
        ///
        /// `true` if this contains value, else `false`.
        /// --------------------------------------------------------------------------------------------
        constexpr auto eq(NullOption) const -> bool
        {
            return isNull();
        }

        /// --------------------------------------------------------------------------------------------
        /// # Not Equality Comparision
        ///
        /// `false` if this contains value, else `true`.
        /// --------------------------------------------------------------------------------------------
        constexpr auto ne(NullOption) const -> bool
        {
            return isValue();
        }

        /// --------------------------------------------------------------------------------------------
        /// # Equality Comparision
        ///
        /// If `this` and `that` are null, returns `true`.
        /// If `this` is null and `that` is not null or vice versa, returns `false`.
        /// If `this` and `that` are not null, returns `this.get() == that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr auto eq(const Option<TThat>& that) const -> bool
            requires(REqualityComparableWith<TVal, TThat>)
        {
            if (isValue() != that.isValue())
                // One is null and one has value.
                return false;

            if (isNull())
                // Both are null.
                return true;

            return get() == that.get();
        }

        /// --------------------------------------------------------------------------------------------
        /// # Not Equality Comparision
        ///
        /// Performs negation of [Equality Comparision].
        /// --------------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr auto ne(const Option<TThat>& that) const -> bool
            requires(REqualityComparableWith<TVal, TThat>)
        {
            return not eq(that);
        }

        /// --------------------------------------------------------------------------------------------
        /// # Less Than Comparision
        ///
        /// If `this` or `that` is null, returns false.
        /// Else, returns `this.get() < that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr auto lt(const Option<TThat>& that) const -> bool
            requires(RComparableWith<TVal, TThat>)
        {
            if (isNull() or that.isNull())
                return false;

            return get() < that.get();
        }

        /// --------------------------------------------------------------------------------------------
        /// # Greater Than Comparision
        ///
        /// If `opt0` or `that` is null, returns false.
        /// Else, returns `this.get() > that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr auto gt(const Option<TThat>& that) const -> bool
            requires(RComparableWith<TVal, TThat>)
        {
            if (isNull() or that.isNull())
                return false;

            return get() > that.get();
        }

        /// --------------------------------------------------------------------------------------------
        /// # Less Than or Equal To Comparision
        ///
        /// If `opt0` or `that` is null, returns false.
        /// Else, returns `this.get() <= that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr auto le(const Option<TThat>& that) const -> bool
            requires(RComparableWith<TVal, TThat>)
        {
            if (isNull() or that.isNull())
                return false;

            return get() <= that.get();
        }

        /// --------------------------------------------------------------------------------------------
        /// # Greater Than or Equal To Comparision
        ///
        /// If `opt0` or `that` is null, returns false.
        /// Else, returns `this.get() >= that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr auto ge(const Option<TThat>& that) const -> bool
            requires(RComparableWith<TVal, TThat>)
        {
            if (isNull() or that.isNull())
                return false;

            return get() >= that.get();
        }

    private:
        Impl _impl;
    };
}
