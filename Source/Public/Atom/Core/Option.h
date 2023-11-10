#pragma once
#include "Atom/Invokable/Invokable.h"
#include "_OptionImpl.h"

namespace Atom
{
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
        static_assert(not TTI::IsVoid<T>, "Option doesn't support void type.");

        static_assert(
            not TTI::IsRef<T>, "Option doesn't support reference types, instead use pointers.");

        static_assert(not TTI::IsQualified<T>, "Option doesn't support qualified types.");

    private:
        using This = Option<T>;
        using _TImpl = _OptionImpl<T>;

    public:
        using TVal = T;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        ///
        /// Constructs with no value. [`isValue()`] will return false.
        /// ----------------------------------------------------------------------------------------
        constexpr Option():
            _impl()
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
        /// - If `that` contains value, copy constructs `this` value with `that` value.
        /// - Else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr Option(const Option& that)
            requires(not RTriviallyCopyConstructible<TVal>) and (RCopyConstructible<TVal>)
            :
            _impl(that._impl)
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
        /// - If `that` contains value
        ///     - If `this` contains value, copy assigns with `that` value.
        ///     - Else, copy constructs with `that` value.
        /// - Else
        ///     - If `this` contains value, destroys `this` value.
        ///     - Else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const Option& that) -> Option&
            requires(not RTriviallyCopyAssignable<TVal>) and (RCopyable<TVal>)
        {
            _impl.assignValueFromOption(that._impl);
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
            :
            _impl(mov(that._impl))
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
        /// - If `that` contains value
        ///     - If `this` contains value, move assigns with `that` value.
        ///     - Else, move constructs with `that` value.
        /// - Else
        ///     - If `this` contains value, destroys `this` value.
        ///     - Else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(Option&& that) -> Option&
            requires(not RTriviallyMoveAssignable<TVal>) and (RMoveable<TVal>)
        {
            _impl.assignValueFromOption(mov(that._impl));
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
        constexpr Option(const TVal& val):
            _impl(val)
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
        constexpr auto operator=(const TVal& val) -> Option&
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
        constexpr Option(TVal&& val):
            _impl(mov(val))
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
        constexpr auto operator=(TVal&& val) -> Option&
        {
            _impl.assignValue(mov(val));
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
        {}

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
            requires(RConstructible<TVal, TArgs...>)
        {
            _impl.emplaceValue(forward<TArgs>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto value() & -> TVal&
        {
            expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto value() const& -> const TVal&
        {
            expects(isValue(), "Doesn't contain value.");

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto value() && -> TVal&&
        {
            expects(isValue(), "Doesn't contain value.");

            return mov(_impl.getValue());
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() -> TVal*
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return &_impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() const -> const TVal*
        {
            debug_expects(isValue(), "Doesn't contain value.");

            return &_impl.getValue();
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
        constexpr auto valueOr(const TVal& orVal) const -> const TVal&
        {
            if (_impl.isNull())
            {
                return orVal;
            }

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto valueOr(TVal& orVal) -> TVal&
        {
            if (_impl.isNull())
            {
                return orVal;
            }

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto valueOr(TVal&& orVal) && -> TVal&&
        {
            if (_impl.isNull())
            {
                return mov(orVal);
            }

            return mov(_impl.getValue());
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
        constexpr auto valueOrInvoke(TInvokable&& orInvoke) const -> const TVal&
            requires RInvokable<TPure<TInvokable>, const TVal&()>
        {
            if (_impl.isNull())
            {
                return orInvoke();
            }

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        constexpr auto valueOrInvoke(TInvokable&& orInvoke) -> TVal&
            requires RInvokable<TPure<TInvokable>, TVal&()>
        {
            if (_impl.isNull())
            {
                return orInvoke();
            }

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        constexpr auto valueOrInvoke(TInvokable&& orInvoke) && -> TVal&&
            requires RInvokable<TPure<TInvokable>, TVal&&()>
        {
            if (_impl.isNull())
            {
                return orInvoke();
            }

            return mov(_impl.getValue());
        }

        /// ----------------------------------------------------------------------------------------
        /// Get `this` value or default.
        ///
        /// - If `this` contains value, get `this` value.
        /// - Else, get default constructed value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto valueOrDefault() const& -> const TVal&
            requires(RDefaultConstructible<TVal>)
        {
            if (_impl.isNull())
            {
                return _TImpl::GetDefault();
            }

            return _impl.getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto valueOrDefault() && -> TVal&&
            requires(RDefaultConstructible<TVal>)
        {
            if (_impl.isNull())
            {
                return mov(_TImpl::GetDefault());
            }

            return mov(_impl.getValue());
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

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Compairision.
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        /// --------------------------------------------------------------------------------------------
        /// # Equality Comparision Operator
        ///
        /// - If `this` and `that` are null, returns `true`.
        /// - If `this` is null and `that` is not null or vice versa, returns `false`.
        /// - If `this` and `that` are not null, returns `this.value() == that.value()`.
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

            return value() == that.value();
        }

        /// --------------------------------------------------------------------------------------------
        /// # Not Equality Comparision Operator
        ///
        /// Performs negation of [Equality Comparision Operator].
        /// --------------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr auto ne(const Option<TThat>& that) const -> bool
            requires(REqualityComparableWith<TVal, TThat>)
        {
            return not eq(that);
        }

        /// --------------------------------------------------------------------------------------------
        /// # Less Than Comparision Operator
        ///
        /// - If `this` or `that` is null, returns false.
        /// - Else, returns `this.value() < that.value()`.
        /// --------------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr auto lt(const Option<TThat>& that) const -> bool
            requires(RComparableWith<TVal, TThat>)
        {
            if (isNull() or that.isNull())
                return false;

            return value() < that.value();
        }

        /// --------------------------------------------------------------------------------------------
        /// # Greater Than Comparision Operator
        ///
        /// - If `opt0` or `that` is null, returns false.
        /// - Else, returns `this.value() > that.value()`.
        /// --------------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr auto gt(const Option<TThat>& that) const -> bool
            requires(RComparableWith<TVal, TThat>)
        {
            if (isNull() or that.isNull())
                return false;

            return value() > that.value();
        }

        /// --------------------------------------------------------------------------------------------
        /// # Less Than or Equal To Comparision Operator
        ///
        /// - If `opt0` or `that` is null, returns false.
        /// - Else, returns `this.value() <= that.value()`.
        /// --------------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr auto le(const Option<TThat>& that) const -> bool
            requires(RComparableWith<TVal, TThat>)
        {
            if (isNull() or that.isNull())
                return false;

            return value() <= that.value();
        }

        /// --------------------------------------------------------------------------------------------
        /// # Greater Than or Equal To Comparision Operator
        ///
        /// - If `opt0` or `that` is null, returns false.
        /// - Else, returns `this.value() >= that.value()`.
        /// --------------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr auto ge(const Option<TThat>& that) const -> bool
            requires(RComparableWith<TVal, TThat>)
        {
            if (isNull() or that.isNull())
                return false;

            return value() >= that.value();
        }

    private:
        _TImpl _impl;
    };
}
