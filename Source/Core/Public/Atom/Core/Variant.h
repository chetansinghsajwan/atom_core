#pragma once
#include "_VariantImpl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// # To Do
    /// - Check Requirements for assignments.
    /// - Check if requirements using TypeList functionality can be made concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    requires(TypeList<Ts...>::AreUnique)
        and (TypeList<Ts...>::Count > 0)
        and (not TypeList<Ts...>::template Has<void>)
    class Variant
    {
    private:
        using _Impl = _VariantImpl<Ts...>;

        template <typename... TOthers>
        requires(TypeList<TOthers...>::AreUnique)
            and (TypeList<TOthers...>::Count > 0)
            and (not TypeList<TOthers...>::template Has<void>)
        friend class Variant;

        using Self = Variant<Ts...>;

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
        template <typename T>
        static consteval auto Has() -> bool
        {
            return _Impl::template HasType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Check if index `i` can be used to access value.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        static consteval auto Has() -> bool
        {
            return _Impl::template HasIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get type at index.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        requires(Has<i>())
        using TAt = typename _Impl::template TypeAtIndex<i>;

        /// ----------------------------------------------------------------------------------------
        /// Index of type. This index than can be used to access value of type at that index.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        static consteval auto IndexOf() -> usize
            requires(Has<T>())
        {
            return _Impl::template GetIndexForType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Count of types this variant supports.
        /// ----------------------------------------------------------------------------------------
        static consteval auto count() -> usize
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
        constexpr Variant()
            requires(RDefaultConstructible<TAt<0>>)
        {
            _impl.template constructValueByIndex<0>();
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Variant(const Variant& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Variant(const Variant& that)
            requires(RCopyConstructibleAll<Ts...>)
                and (not RTriviallyCopyConstructibleAll<Ts...>)
        {
            _impl.constructValueFromVariant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor Template
        /// ----------------------------------------------------------------------------------------
        template <typename... TOthers>
        constexpr Variant(const Variant<TOthers...>& that)
            requires(Types::template Has<TOthers...>)
                and (RCopyConstructibleAll<TOthers...>)
        {
            _impl.constructValueFromVariant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Assignment Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const Variant& that) -> Variant& = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Assignment Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const Variant& that) -> Variant&
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
        template <typename... TOthers>
        constexpr auto operator=(const Variant<TOthers...>& that) -> Variant&
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
        constexpr Variant(Variant&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Variant(Variant&& that)
            requires(RMoveConstructibleAll<Ts...>)
                and (not RTriviallyMoveConstructibleAll<Ts...>)
        {
            _impl.constructValueFromVariant(mov(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor Template
        /// ----------------------------------------------------------------------------------------
        template <typename... TOthers>
        constexpr Variant(Variant<TOthers...>&& that)
            requires(Types::template Has<TOthers...>)
                and (RMoveConstructibleAll<TOthers...>)
        {
            _impl.constructValueFromVariant(mov(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Assignment Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(Variant&& that) -> Variant& = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Assignment Operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(Variant&& that) -> Variant&
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
        template <typename... TOthers>
        constexpr auto operator=(Variant<TOthers...>&& that) -> Variant&
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
        template <typename T>
        constexpr Variant(const T& value)
            requires(Has<T>())
        {
            _impl.template constructValueByType<T>(value);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Move Constructor
        /// 
        /// Move constructs variant with value of type T.
        /// 
        /// # Parameters
        /// - `value`: Value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr Variant(T&& value)
            requires(Has<T>())
        {
            _impl.template constructValueByType<T>(mov(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Copy Operator
        /// 
        /// Destroys previous value assigns new value.
        /// 
        /// # Parameters
        /// - `value`: Value to assign.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto operator=(const T& value) -> Variant&
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
        template <typename T>
        constexpr auto operator=(T&& value) -> Variant&
            requires(Has<T>())
        {
            _impl.setValue(mov(value));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~Variant() = default;

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// 
        /// Destructs value.
        /// ----------------------------------------------------------------------------------------
        constexpr ~Variant()
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
        template <typename T, typename... TArgs>
        constexpr auto emplace(TArgs&&... args)
            requires(Has<T>())
                and (RConstructible<T, TArgs...>)
        {
            _impl.template emplaceValueByType<T>(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs the type for index `i` and sets the value.
        /// 
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename... TArgs>
        constexpr auto emplace(TArgs&&... args)
            requires(Has<i>())
                and (RConstructible<TAt<i>, TArgs...>)
        {
            _impl.template emplaceValueByIndex<i>(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the value to `value`.
        /// 
        /// # Parameters
        /// - `value`: Value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename TFwd, typename T = TTI::TRemoveQuailfiersRef<TFwd>>
        constexpr auto set(TFwd&& value)
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
        template <typename T>
        constexpr auto as() const -> const T&
            requires(Has<T>())
        {
            expects(is<T>(), "Access to invalid type.");

            return _impl.template getValueByType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value as type `T`.
        /// 
        /// # Template Parameters
        /// - `T`: Type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto as() -> T&
            requires(Has<T>())
        {
            expects(is<T>(), "Access to invalid type.");

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
        constexpr auto at() const -> const TAt<i>&
            requires(Has<i>())
        {
            expects(is<i>(), "Access to invalid type by index.");

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
        constexpr auto at() -> TAt<i>&
            requires(Has<i>())
        {
            expects(is<i>(), "Access to invalid type by index.");

            return _impl.template getValueByIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if current value is of type `T`.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto is() const -> bool
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
        constexpr auto is() const -> bool
            requires(Has<i>())
        {
            return _impl.template isIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the index to current type.
        /// ----------------------------------------------------------------------------------------
        constexpr auto index() const -> usize
        {
            return _impl.getTypeIndex();
        }

    private:
        _Impl _impl;
    };
}