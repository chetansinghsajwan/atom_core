#pragma once
#include "Atom/Core.h"
#include "Atom/Memory/ObjHelper.decl.h"
#include "Atom/TTI.h"
#include "Storage.h"
#include "TypeList.h"

namespace Atom
{
    template <typename... Ts>
    class _VariantStorage
    {
    public:
        constexpr auto getData() -> void*
        {
            return &_storage.storage;
        }

        constexpr auto getData() const -> const void*
        {
            return &_storage.storage;
        }

    private:
        AlignedUnionStorageFor<Ts...> _storage;
    };

    /// --------------------------------------------------------------------------------------------
    /// Implementatiion of [`Variant`].
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    class _VariantImpl
    {
    private:
        template <typename... TOthers>
        friend class _VariantImpl;

    private:
        using _Types = TypeList<Ts...>;
        using _Storage = _VariantStorage<Ts...>;

        ////
        ///-------------------------------------------------------------------------------------------
        //// Static Functions
        ////
        ///-------------------------------------------------------------------------------------------

    public:
        static constexpr auto GetTypeCount() -> usize
        {
            return _Types::Count;
        }

        template <typename T>
        static consteval auto HasType() -> bool
        {
            return _Types::template Has<T>;
        }

        template <usize i>
        static consteval auto HasIndex() -> bool
        {
            return i < _Types::Count;
        }

        template <typename T>
        static consteval auto GetIndexForType() -> usize
        {
            return _Types::template IndexOf<T>;
        }

        static consteval auto GetNullTypeIndex() -> usize
        {
            return -1;
        }

        template <usize i>
        using TypeAtIndex = typename _Types::template At<i>;

        ////
        ///-------------------------------------------------------------------------------------------
        //// Funcions
        ////
        ///-------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Copy or move constructs value hold by variant `that`.
        ///
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename... TOthers>
        constexpr auto constructValueFromVariant(const _VariantImpl<TOthers...>& that)
        {
            _constructValueFromVariantImpl<false, 0, TOthers...>(that, that.getTypeIndex());
        }

        template <typename... TOthers>
        constexpr auto constructValueFromVariant(_VariantImpl<TOthers...>&& that)
        {
            _constructValueFromVariantImpl<true, 0, TOthers...>(that, that.getTypeIndex());
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs or assigns value from `that` variant.
        /// Assigns if `that` variant holds the same type else constructs.
        /// ----------------------------------------------------------------------------------------
        template <typename... TOthers>
        constexpr auto setValueFromVariant(const _VariantImpl<TOthers...>& that)
        {
            _setValueFromVariantImpl<false, 0, TOthers...>(that, that.getTypeIndex());
        }

        template <typename... TOthers>
        constexpr auto setValueFromVariant(_VariantImpl<TOthers...>&& that)
        {
            _setValueFromVariantImpl<true, 0, TOthers...>(that, that.getTypeIndex());
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs value of type `T` with args `args`.
        ///
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename T, typename... TArgs>
        constexpr auto constructValueByType(TArgs&&... args)
        {
            _constructValueAs<T>(fwd(args)...);
            _index = GetIndexForType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs value of type at index `i` with args `args`.
        ///
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename... TArgs>
        constexpr auto constructValueByIndex(TArgs&&... args)
        {
            constructValueByType<TypeAtIndex<i>>(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs value of type `T` with args `args`.
        ///
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename T, typename... TArgs>
        constexpr auto emplaceValueByType(TArgs&&... args)
        {
            destroyValue();

            _constructValueAs<T>(fwd(args)...);
            _index = GetIndexForType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs value of type at index `i` with args `args`.
        ///
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename... TArgs>
        constexpr auto emplaceValueByIndex(TArgs&&... args)
        {
            emplaceValueByType<TypeAtIndex<i>>(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs or assigns value of type deduced by `value` with `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename TFwd>
        constexpr auto setValue(TFwd&& value)
        {
            using T = TTI::TRemoveQuailfiersRef<TFwd>;
            usize indexToSet = GetIndexForType<T>();

            // The new type to set is same as current.
            if (indexToSet == _index)
            {
                _assignValueAs<T>(fwd(value));
            }
            else
            {
                destroyValue();
                constructValueByType<T>(fwd(value));
            }
        }

        template <typename T>
        constexpr auto getValueByType() const -> const T&
        {
            debug_expects(GetIndexForType<T>() == getTypeIndex(),
                "Current type is not same as requested type.");

            return _getValueAs<T>();
        }

        template <typename T>
        constexpr auto getValueByType() -> T&
        {
            debug_expects(GetIndexForType<T>() == getTypeIndex(),
                "Current type is not same as requested type.");

            return _getValueAs<T>();
        }

        template <usize i>
        constexpr auto getValueByIndex() const -> const TypeAtIndex<i>&
        {
            return getValueByType<TypeAtIndex<i>>();
        }

        template <usize i>
        constexpr auto getValueByIndex() -> TypeAtIndex<i>&
        {
            return getValueByType<TypeAtIndex<i>>();
        }

        template <typename T>
        constexpr auto isType() const -> bool
        {
            return GetIndexForType<T>() == getTypeIndex();
        }

        template <usize i>
        constexpr auto isIndex() const -> bool
        {
            return i == getTypeIndex();
        }

        constexpr auto getTypeIndex() const -> usize
        {
            return _index;
        }

        constexpr auto destroyValue()
        {
            _destroyValueImpl<0, Ts...>(getTypeIndex());
        }

    private:
        template <bool move, usize index, typename TOther, typename... TOthers>
        constexpr auto _constructValueFromVariantImpl(auto& that, usize thatIndex)
        {
            using ThatTypes = TypeList<TOthers...>;

            if (index != thatIndex)
            {
                if constexpr (ThatTypes::Count == 0)
                {
                    panic("There is no type for current index.");
                }
                else
                {
                    _constructValueFromVariantImpl<move, index + 1, TOthers...>(that, thatIndex);
                    return;
                }
            }

            if constexpr (move)
            {
                _constructValueAs<TOther>(mov(that.template _getValueAs<TOther>()));
            }
            else
            {
                _constructValueAs<TOther>(that.template _getValueAs<TOther>());
            }

            _index = GetIndexForType<TOther>();
        }

        template <bool move, usize index, typename TOther, typename... TOthers>
        constexpr auto _setValueFromVariantImpl(auto&& that, usize thatIndex)
        {
            using ThatTypes = TypeList<TOthers...>;

            if (index != thatIndex)
            {
                if constexpr (ThatTypes::Count == 0)
                {
                    panic("There is no type for current index.");
                }
                else
                {
                    _setValueFromVariantImpl<move, index + 1, TOthers...>(that, thatIndex);
                    return;
                }
            }

            // Index for this variant of type same as that `variant` current type.
            usize indexForThis = GetIndexForType<TOther>();

            // We already have this type, so we don't construct it but assign it.
            if (_index == indexForThis)
            {
                if constexpr (move)
                {
                    _assignValueAs<TOther>(mov(that.template _getValueAs<TOther>()));
                }
                else
                {
                    _assignValueAs<TOther>(that.template _getValueAs<TOther>());
                }
            }
            else
            {
                destroyValue();

                if constexpr (move)
                {
                    _constructValueAs<TOther>(mov(that.template _getValueAs<TOther>()));
                }
                else
                {
                    _constructValueAs<TOther>(that.template _getValueAs<TOther>());
                }

                _index = indexForThis;
            }
        }

        template <usize index, typename T, typename... Ts_>
        constexpr auto _destroyValueImpl(usize i)
        {
            using Types = TypeList<Ts_...>;

            if (i != index)
            {
                if constexpr (Types::Count == 0)
                {
                    panic("There is no type for current index.");
                }
                else
                {
                    // Recursion to find type at index i.
                    _destroyValueImpl<index + 1, Ts_...>(i);
                    return;
                }
            }

            _destructValueAs<T>();
        }

        template <typename T>
        constexpr auto _constructValueAs(auto&&... args)
        {
            ObjHelper().Construct(_getDataAs<T>(), fwd(args)...);
        }

        template <typename T>
        constexpr auto _assignValueAs(auto&& val)
        {
            ObjHelper().Assign(_getDataAs<T>(), fwd(val));
        }

        template <typename T>
        constexpr auto _destructValueAs()
        {
            ObjHelper().Destruct(_getDataAs<T>());
        }

        template <typename T>
        constexpr auto _getValueAs() -> T&
        {
            return *_getDataAs<T>();
        }

        template <typename T>
        constexpr auto _getValueAs() const -> const T&
        {
            return *_getDataAs<T>();
        }

        template <typename T>
        constexpr auto _getDataAs() -> T*
        {
            return reinterpret_cast<T*>(_storage.getData());
        }

        template <typename T>
        constexpr auto _getDataAs() const -> const T*
        {
            return reinterpret_cast<const T*>(_storage.getData());
        }

        ////
        ///-------------------------------------------------------------------------------------------
        //// Fields
        ////
        ///-------------------------------------------------------------------------------------------

    private:
        _Storage _storage;
        usize _index = 0;
    };
}
