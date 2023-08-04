#pragma once
#include "_VariantImpl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// # To Do
    /// - Check Requirements for assignments.
    /// - Check if requirements using TypeList functionality can be made concepts.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    requires(TypeList<Ts...>::AreUnique)
        and (TypeList<Ts...>::Count > 0)
        and (not TypeList<Ts...>::template Has<void>)
    class Variant
    {
    private:
        using _Impl = _VariantImpl<Ts...>;

        template <tname... TOthers>
        requires(TypeList<TOthers...>::AreUnique)
            and (TypeList<TOthers...>::Count > 0)
            and (not TypeList<TOthers...>::template Has<void>)
        friend class Variant;

        using This = Variant<Ts...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// TypeList of this variant.
        /// ----------------------------------------------------------------------------------------
        using Types = TypeList<Ts...>;

    //// -------------------------------------------------------------------------------------------
    //// Static Functions
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Check if this variant supports type `T`.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        static ceval fn Has() -> bool
        {
            return _Impl::template HasType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Check if index `i` can be used to access value.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        static ceval fn Has() -> bool
        {
            return _Impl::template HasIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get type at index.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        requires(Has<i>())
        using TAt = tname _Impl::template TypeAtIndex<i>;

        /// ----------------------------------------------------------------------------------------
        /// Index of type. This index than can be used to access value of type at that index.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        static ceval fn IndexOf() -> usize
            requires(Has<T>())
        {
            return _Impl::template GetIndexForType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Count of types this variant supports.
        /// ----------------------------------------------------------------------------------------
        static ceval fn Count() -> usize
        {
            return _Impl::GetTypeCount();
        }

    //// -------------------------------------------------------------------------------------------
    //// Constructors, Assignments and Destructor.
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        cexpr ctor Variant()
            requires(RDefaultConstructible<TAt<0>>)
        {
            _impl.template emplaceValueByIndex<0>();
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Constructor
        /// ----------------------------------------------------------------------------------------
        cexpr ctor Variant(const Variant& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        cexpr ctor Variant(const Variant& that)
            requires(RCopyConstructibleAll<Ts...>)
                and (not RTriviallyCopyConstructibleAll<Ts...>)
        {
            _impl.emplaceValueFromVariant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor Template
        /// ----------------------------------------------------------------------------------------
        template <tname... TOthers>
        cexpr ctor Variant(const Variant<TOthers...>& that)
            requires(Types::template Has<TOthers...>)
                and (RCopyConstructibleAll<TOthers...>)
        {
            _impl.emplaceValueFromVariant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Assignment Operator
        /// ----------------------------------------------------------------------------------------
        cexpr fn op=(const Variant& that) -> Variant& = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Assignment Operator
        /// ----------------------------------------------------------------------------------------
        cexpr fn op=(const Variant& that) -> Variant&
            requires(RCopyConstructibleAll<Ts...>)
                and (RCopyAssignableAll<Ts...>)
                and (not RTriviallyCopyConstructibleAll<Ts...>)
                and (not RTriviallyCopyAssignableAll<Ts...>)
        {
            _impl.setValueFromVariant(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Copy Assignment Operator Template
        /// ----------------------------------------------------------------------------------------
        template <tname... TOthers>
        cexpr fn op=(const Variant<TOthers...>& that) -> Variant&
            requires(Types::template Has<TOthers...>)
                and (RCopyConstructibleAll<TOthers...>)
                and (RCopyAssignableAll<TOthers...>)
        {
            _impl.setValueFromVariant(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Constructor
        /// ----------------------------------------------------------------------------------------
        cexpr ctor Variant(Variant&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        cexpr ctor Variant(Variant&& that)
            requires(RMoveConstructibleAll<Ts...>)
                and (not RTriviallyMoveConstructibleAll<Ts...>)
        {
            _impl.emplaceValueFromVariant(mov(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor Template
        /// ----------------------------------------------------------------------------------------
        template <tname... TOthers>
        cexpr ctor Variant(Variant<TOthers...>&& that)
            requires(Types::template Has<TOthers...>)
                and (RMoveConstructibleAll<TOthers...>)
        {
            _impl.emplaceValueFromVariant(mov(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Assignment Operator
        /// ----------------------------------------------------------------------------------------
        cexpr fn op=(Variant&& that) -> Variant& = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Assignment Operator
        /// ----------------------------------------------------------------------------------------
        cexpr fn op=(Variant&& that) -> Variant&
            requires(RMoveConstructibleAll<Ts...>)
                and (RMoveAssignableAll<Ts...>)
                and (not RTriviallyMoveConstructibleAll<Ts...>)
                and (not RTriviallyMoveAssignableAll<Ts...>)
        {
            _impl.setValueFromVariant(mov(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Move Assignment Operator Template
        /// ----------------------------------------------------------------------------------------
        template <tname... TOthers>
        cexpr fn op=(Variant<TOthers...>&& that) -> Variant&
            requires(Types::template Has<TOthers...>)
                and (RMoveConstructibleAll<Ts...>)
                and (RMoveAssignableAll<Ts...>)
        {
            _impl.setValueFromVariant(mov(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Copy Constructor
        /// 
        /// Copy constructs variant with value of type T.
        /// 
        /// # Parameters
        /// - `value`: Value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        cexpr ctor Variant(const T& value)
            requires(Has<T>())
        {
            _impl.template emplaceValueByType<T>(value);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Move Constructor
        /// 
        /// Move constructs variant with value of type T.
        /// 
        /// # Parameters
        /// - `value`: Value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        cexpr ctor Variant(T&& value)
            requires(Has<T>())
        {
            _impl.template emplaceValueByType<T>(mov(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Copy Operator
        /// 
        /// Destroys previous value assigns new value.
        /// 
        /// # Parameters
        /// - `value`: Value to assign.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        cexpr fn op=(const T& value) -> Variant&
            requires(Has<T>())
        {
            _impl.setValue(value);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Move Operator
        /// 
        /// Destroys previous value assigns new value.
        /// 
        /// # Parameters
        /// - `value`: Value to assign.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        cexpr fn op=(T&& value) -> Variant&
            requires(Has<T>())
        {
            _impl.setValue(mov(value));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Destructor
        /// ----------------------------------------------------------------------------------------
        cexpr dtor Variant() = default;

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// 
        /// Destructs value.
        /// ----------------------------------------------------------------------------------------
        cexpr dtor Variant()
            requires(not RTriviallyDestructibleAll<Ts...>)
        {
            _impl.destroyValue();
        }

    //// -------------------------------------------------------------------------------------------
    //// Functions
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Constructs the type `T` and sets the value.
        /// 
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        template <tname T, tname... TArgs>
        cexpr fn emplace(TArgs&&... args)
            requires(Has<T>())
                and (RConstructible<T, TArgs...>)
        {
            _impl.destroyValue();
            _impl.template emplaceValueByType<T>(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs the type for index `i` and sets the value.
        /// 
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        template <usize i, tname... TArgs>
        cexpr fn emplace(TArgs&&... args)
            requires(Has<i>())
                and (RConstructible<TAt<i>, TArgs...>)
        {
            _impl.destroyValue();
            _impl.template emplaceByIndex<i>(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the value to `value`.
        /// 
        /// # Parameters
        /// - `value`: Value to set.
        /// ----------------------------------------------------------------------------------------
        template <tname TFwd, tname T = TTI::TRemoveQuailfiersRef<TFwd>>
        cexpr fn set(TFwd&& value)
            requires(Has<T>())
                and (RConstructible<T, TFwd>)
        {
            _impl.setValue(fwd(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value as type `T`.
        /// 
        /// # Template Parameters
        /// - `T`: Type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        cexpr fn as() const -> const T&
            requires(Has<T>())
        {
            CONTRACTS_EXPECTS(is<T>(), "Access to invalid type.");

            return _impl.template getValueByType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value as type `T`.
        /// 
        /// # Template Parameters
        /// - `T`: Type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        cexpr fn as() -> T&
            requires(Has<T>())
        {
            CONTRACTS_EXPECTS(is<T>(), "Access to invalid type.");

            return _impl.template getValueByType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value at index `i`.
        /// 
        /// # Template Parameters
        /// - `i`: Index of type to access variants value as.
        /// 
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        cexpr fn at() const -> const TAt<i>&
            requires(Has<i>())
        {
            CONTRACTS_EXPECTS(is<i>(), "Access to invalid type by index.");

            return _impl.template getValueByIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value at index `i`.
        /// 
        /// # Template Parameters
        /// - `i`: Index of type to access variants value as.
        /// 
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        cexpr fn at() -> TAt<i>&
            requires(Has<i>())
        {
            CONTRACTS_EXPECTS(is<i>(), "Access to invalid type by index.");

            return _impl.template getValueByIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if current value is of type `T`.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        cexpr fn is() const -> bool
            requires(Has<T>())
        {
            return _impl.template isType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if current value is of type accessed by index `i`.
        /// 
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        cexpr fn is() const -> bool
            requires(Has<i>())
        {
            return _impl.template isIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the index to current type.
        /// ----------------------------------------------------------------------------------------
        cexpr fn index() const -> usize
        {
            return _impl.getTypeIndex();
        }

    private:
        _Impl _impl;
    };
}