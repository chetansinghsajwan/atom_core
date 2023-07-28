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
        and (not TypeList<Ts...>::template Has<void>)
    class Variant: private _VariantImpl<Ts...>
    {
    private:
        template <tname... TOthers>
        requires(TypeList<TOthers...>::AreUnique)
            and (not TypeList<TOthers...>::template Has<void>)
        friend class Variant;

        using This = Variant<Ts...>;
        using ImplBase = _VariantImpl<Ts...>;

        /// ----------------------------------------------------------------------------------------
        /// TypeList of this variant.
        /// ----------------------------------------------------------------------------------------
        pub using Types = TypeList<Ts...>;

    //// -------------------------------------------------------------------------------------------
    //// Static Functions
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        /// Check if this variant supports type `T`.
        /// ----------------------------------------------------------------------------------------
        pub template <tname T>
        static ceval fn Has() -> bool
        {
            return ImplBase::template _HasType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Check if index `i` can be used to access value.
        /// ----------------------------------------------------------------------------------------
        pub template <usize i>
        static ceval fn Has() -> bool
        {
            return ImplBase::template _HasIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get type at index.
        /// ----------------------------------------------------------------------------------------
        pub template <usize i>
        requires (Has<i>())
        using TAt = tname ImplBase::template _TypeAtIndex<i>;

        /// ----------------------------------------------------------------------------------------
        /// Index of type. This index than can be used to access value of type at that index.
        /// ----------------------------------------------------------------------------------------
        pub template <tname T>
        requires (Has<T>())
        static ceval fn IndexOf() -> usize
        {
            return ImplBase::template _GetIndexForType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Count of types this variant supports.
        /// ----------------------------------------------------------------------------------------
        pub static ceval fn Count() -> usize
        {
            return ImplBase::_TypeCount;
        }

    //// -------------------------------------------------------------------------------------------
    //// Constructors, Assignments and Destructor.
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Trivial Default Constructor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor Variant() = default;

        /// ----------------------------------------------------------------------------------------
        /// Default Constructor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor Variant()
        requires(RDefaultConstructible<TAt<0>>)
            and (not RTriviallyDefaultConstructible<TAt<0>>)
        {
            ImplBase::template _emplaceValueByIndex<0>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Trivial Copy Constructor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor Variant(const Variant& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// Copy Constructor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor Variant(const Variant& that)
        requires(RCopyConstructibleAll<Ts...>)
            and (not RTriviallyCopyConstructibleAll<Ts...>)
        {
            ImplBase::_emplaceValueFromVariant(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Copy Constructor Template.
        /// ----------------------------------------------------------------------------------------
        template <tname... TOthers>
        cexpr ctor Variant(const Variant<TOthers...>& that)
        requires(Types::template Has<TOthers...>)
            and (RCopyConstructibleAll<TOthers...>)
        {
            ImplBase::_emplaceValueFromVariant(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Trivial Copy Assignment Operator.
        /// ----------------------------------------------------------------------------------------
        cexpr fn op=(const Variant& that) -> Variant& = default;

        /// ----------------------------------------------------------------------------------------
        /// Copy Assignment Operator.
        /// ----------------------------------------------------------------------------------------
        cexpr fn op=(const Variant& that) -> Variant&
        requires(RCopyConstructibleAll<Ts...>)
            and (RCopyAssignableAll<Ts...>)
            and (not RTriviallyCopyConstructibleAll<Ts...>)
            and (not RTriviallyCopyAssignableAll<Ts...>)
        {
            ImplBase::_setValueFromVariant(that);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Copy Assignment Operator Template.
        /// ----------------------------------------------------------------------------------------
        template <tname... TOthers>
        cexpr fn op=(const Variant<TOthers...>& that) -> Variant&
        requires(Types::template Has<TOthers...>)
            and (RCopyConstructibleAll<TOthers...>)
            and (RCopyAssignableAll<TOthers...>)
        {
            ImplBase::_setValueFromVariant(that);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Trivial Move Constructor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor Variant(Variant&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// Move Constructor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor Variant(Variant&& that)
        requires(RMoveConstructibleAll<Ts...>)
            and (not RTriviallyMoveConstructibleAll<Ts...>)
        {
            ImplBase::_emplaceValueFromVariant(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// Move Constructor Template.
        /// ----------------------------------------------------------------------------------------
        template <tname... TOthers>
        cexpr ctor Variant(Variant<TOthers...>&& that)
        requires(Types::template Has<TOthers...>)
            and (RMoveConstructibleAll<TOthers...>)
        {
            ImplBase::_emplaceValueFromVariant(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// Trivial Move Assignment Operator.
        /// ----------------------------------------------------------------------------------------
        cexpr fn op=(Variant&& that) -> Variant& = default;

        /// ----------------------------------------------------------------------------------------
        /// Move Assignment Operator.
        /// ----------------------------------------------------------------------------------------
        cexpr fn op=(Variant&& that) -> Variant&
        requires(RMoveConstructibleAll<Ts...>)
            and (RMoveAssignableAll<Ts...>)
            and (not RTriviallyMoveConstructibleAll<Ts...>)
            and (not RTriviallyMoveAssignableAll<Ts...>)
        {
            ImplBase::_setValueFromVariant(mov(that));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Move Assignment Operator Template.
        /// ----------------------------------------------------------------------------------------
        template <tname... TOthers>
        cexpr fn op=(Variant<TOthers...>&& that) -> Variant&
        requires(Types::template Has<TOthers...>)
            and (RMoveConstructibleAll<Ts...>)
            and (RMoveAssignableAll<Ts...>)
        {
            ImplBase::_setValueFromVariant(mov(that));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs variant with value of type T.
        /// 
        /// # Parameters
        /// - `value`: Value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <tname TFwd, tname T = TTI::TRemoveQuailfiersRef<TFwd>>
        cexpr ctor Variant(TFwd&& value)
        requires(Has<T>())
        {
            ImplBase::template _emplaceValueByType<T>(fwd(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroys previous value assigns new value.
        /// 
        /// # Parameters
        /// - `value`: Value to assign.
        /// ----------------------------------------------------------------------------------------
        template <tname TFwd, tname T = TTI::TRemoveQuailfiersRef<TFwd>>
        cexpr fn op=(TFwd&& value) -> Variant&
        requires(Has<T>())
        {
            ImplBase::_setValue(fwd(value));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Trivial Destructor.
        /// ----------------------------------------------------------------------------------------
        cexpr dtor Variant() = default;

        /// ----------------------------------------------------------------------------------------
        /// Destructor. Destructs value.
        /// ----------------------------------------------------------------------------------------
        cexpr dtor Variant()
        requires(not RTriviallyDestructibleAll<Ts...>)
        {
            ImplBase::_destroyValue();
        }

    //// -------------------------------------------------------------------------------------------
    //// Functions
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        /// Constructs the type `T` and sets the value.
        /// 
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        pub template <tname T, tname... TArgs>
        requires (Has<T>()) and RConstructible<T, TArgs...>
        cexpr fn emplace(TArgs&&... args)
        {
            ImplBase::_destroyValue();
            ImplBase::template _emplaceValueByType<T>(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs the type for index `i` and sets the value.
        /// 
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        pub template <usize i, tname... TArgs>
        requires (Has<i>()) and RConstructible<TAt<i>, TArgs...>
        cexpr fn emplace(TArgs&&... args)
        {
            ImplBase::_destroyValue();
            ImplBase::template _emplaceByIndex<i>(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the value to `value`.
        /// 
        /// # Parameters
        /// - `value`: Value to set.
        /// ----------------------------------------------------------------------------------------
        pub template <tname TFwd, tname T = TTI::TRemoveQuailfiersRef<TFwd>>
        requires (Has<T>()) and RConstructible<T, TFwd>
        cexpr fn set(TFwd&& value)
        {
            ImplBase::_setValue(fwd(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value as type `T`.
        /// 
        /// # Template Parameters
        /// - `T`: Type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        pub template <tname T>
        requires (Has<T>())
        cexpr fn as() const -> const T&
        {
            CONTRACTS_EXPECTS(is<T>(), "Access to invalid type.");

            return ImplBase::template _getValueByType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value as type `T`.
        /// 
        /// # Template Parameters
        /// - `T`: Type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        pub template <tname T>
        requires (Has<T>())
        cexpr fn as() -> T&
        {
            CONTRACTS_EXPECTS(is<T>(), "Access to invalid type.");

            return ImplBase::template _getValueByType<T>();
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
        pub template <usize i>
        requires (Has<i>())
        cexpr fn at() const -> const TAt<i>&
        {
            CONTRACTS_EXPECTS(is<i>(), "Access to invalid type by index.");

            return ImplBase::template _getValueByIndex<i>();
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
        pub template <usize i>
        requires (Has<i>())
        cexpr fn at() -> TAt<i>&
        {
            CONTRACTS_EXPECTS(is<i>(), "Access to invalid type by index.");

            return ImplBase::template _getValueByIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if current value is of type `T`.
        /// ----------------------------------------------------------------------------------------
        pub template <tname T>
        requires (Has<T>())
        cexpr fn is() const -> bool
        {
            return ImplBase::template _isType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if current value is of type accessed by index `i`.
        /// 
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        pub template <usize i>
        requires (Has<i>())
        cexpr fn is() const -> bool
        {
            return ImplBase::template _isIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the index to current type.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn index() const -> usize
        {
            return ImplBase::_getTypeIndex();
        }
    };
}