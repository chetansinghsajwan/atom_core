#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"
#include "Atom/Memory/ObjHelper.h"
#include "TypeList.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Implementatiion of [`Variant`].
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    class _VariantImpl
    {
        priv using _Types = TypeList<Ts...>;

        prot static cexpr usize _TypeCount = _Types::Count;

        prot template <usize i>
        using _TypeAtIndex = tname _Types::template At<i>;

    //// -------------------------------------------------------------------------------------------
    //// Funcions
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        /// Copy or move constructs value hold by variant `that`.
        /// 
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        prot template <tname... TOthers>
        cexpr fn _emplaceValueFromVariant(const _VariantImpl<TOthers...>& that)
        {
            _emplaceValueFromVariantImpl<false, 0, TOthers...>(that, that._getTypeIndex());
        }

        prot template <tname... TOthers>
        cexpr fn _emplaceValueFromVariant(_VariantImpl<TOthers...>&& that)
        {
            _emplaceValueFromVariantImpl<true, 0, TOthers...>(that, that._getTypeIndex());
        }

        /// ----------------------------------------------------------------------------------------
        /// Copy or move constructs value hold by variant `that`.
        /// 
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        priv template <bool move, usize index, tname TOther, tname... TOthers>
        cexpr fn _emplaceValueFromVariantImpl(auto& that, usize thatIndex)
        {
            using ThatTypes = TypeList<TOthers...>;

            if (index != thatIndex)
            {
                if cexpr (ThatTypes::Count == 0)
                {
                    TERMINATE("There is no type for current index.");
                }
                else
                {
                    _emplaceValueFromVariantImpl<move, index + 1, TOthers...>(that, thatIndex);
                    return;
                }
            }

            if cexpr (move)
            {
                ObjHelper().Construct(&_getDataAs<TOther>(),
                    mov(that.template _getDataAs<TOther>()));
            }
            else
            {
                ObjHelper().Construct(&_getDataAs<TOther>(),
                    that.template _getDataAs<TOther>());
            }

            _index = _GetIndexForType<TOther>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs or assigns value from `that` variant.
        /// Assigns if `that` variant holds the same type else constructs.
        /// ----------------------------------------------------------------------------------------
        prot template <tname... TOthers>
        cexpr fn _setValueFromVariant(const _VariantImpl<TOthers...>& that)
        {
            _setValueFromVariantImpl<false, 0, TOthers...>(that, that._getTypeIndex());
        }

        prot template <tname... TOthers>
        cexpr fn _setValueFromVariant(_VariantImpl<TOthers...>&& that)
        {
            _setValueFromVariantImpl<true, 0, TOthers...>(mov(that), that._getTypeIndex());
        }

        priv template <bool move, usize index, tname TOther, tname... TOthers>
        cexpr fn _setValueFromVariantImpl(auto&& that, usize thatIndex)
        {
            using ThatTypes = TypeList<TOthers...>;

            if (index != thatIndex)
            {
                if cexpr (ThatTypes::Count == 0)
                {
                    TERMINATE("There is no type for current index.");
                }
                else
                {
                    _setValueFromVariantImpl<move, index + 1, TOthers...>(that, thatIndex);
                    return;
                }
            }

            // Index for this variant of type same as that variant' current type.
            usize indexForThis = _GetIndexForType<TOther>();

            // We already have this type, so we don't construct it but assign it.
            if (indexForThis == _index)
            {
                if cexpr (move)
                {
                    ObjHelper().Assign(&_getDataAs<TOther>(),
                        mov(that.template _getDataAs<TOther>()));
                }
                else
                {
                    ObjHelper().Assign(&_getDataAs<TOther>(),
                        that.template _getDataAs<TOther>());
                }
            }
            else
            {
                _destroyValue();

                if cexpr (move)
                {
                    ObjHelper().Construct(&_getDataAs<TOther>(),
                        mov(that.template _getDataAs<TOther>()));
                }
                else
                {
                    ObjHelper().Construct(&_getDataAs<TOther>(),
                        that.template _getDataAs<TOther>());
                }
            }

            _index = _GetIndexForType<TOther>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs value of type `T` with args `args`.
        /// 
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        prot template <tname T, tname... TArgs>
        cexpr fn _emplaceValueByType(TArgs&&... args)
        {
            ObjHelper().Construct(&_getDataAs<T>(), fwd(args)...);
            _index = _GetIndexForType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs value of type at index `i` with args `args`.
        /// 
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        prot template <usize i, tname... TArgs>
        cexpr fn _emplaceValueByIndex(TArgs&&... args)
        {
            _emplaceValueByType<_TypeAtIndex<i>>(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs or assigns value of type deduced by `value` with `value`.
        /// ----------------------------------------------------------------------------------------
        prot template <tname TFwd>
        cexpr fn _setValue(TFwd&& value)
        {
            using T = TTI::TRemoveQuailfiersRef<TFwd>;
            usize indexToSet = _GetIndexForType<T>();

            // The new type to set is same as current.
            if (indexToSet == _index)
            {
                ObjHelper().Assign(&_getDataAs<T>(), fwd(value));
            }
            else
            {
                _destroyValue();
                _emplaceValueByType<T>(fwd(value));
            }
        }

        prot template <tname T>
        cexpr fn _getValueByType() const -> const T&
        {
            CONTRACTS_DEBUG_EXPECTS(_GetIndexForType<T>() == _getTypeIndex(),
                "Current type is not same as requested type.");

            return _getDataAs<T>();
        }

        prot template <tname T>
        cexpr fn _getValueByType() -> T&
        {
            CONTRACTS_DEBUG_EXPECTS(_GetIndexForType<T>() == _getTypeIndex(),
                "Current type is not same as requested type.");

            return _getDataAs<T>();
        }

        prot template <usize i>
        cexpr fn _getValueByIndex() const -> const _TypeAtIndex<i>&
        {
            return _getValueByType<_TypeAtIndex<i>>();
        }

        prot template <usize i>
        cexpr fn _getValueByIndex() -> _TypeAtIndex<i>&
        {
            return _getValueByType<_TypeAtIndex<i>>();
        }

        prot template <tname T>
        static ceval fn _HasType() -> bool
        {
            return _Types::template Has<T>;
        }

        prot template <usize i>
        static ceval fn _HasIndex() -> bool
        {
            return i < _Types::Count;
        }

        prot template <tname T>
        cexpr fn _isType() const -> bool
        {
            return _GetIndexForType<T>() == _getTypeIndex();
        }

        prot template <usize i>
        cexpr fn _isIndex() const -> bool
        {
            return i == _getTypeIndex();
        }

        prot cexpr fn _getTypeIndex() const -> usize
        {
            return _index;
        }

        prot template <tname T>
        static ceval fn _GetIndexForType() -> usize
        {
            return _Types::template IndexOf<T>;
        }

        prot static ceval fn _GetNullTypeIndex() -> usize
        {
            return -1;
        }

        prot cexpr fn _destroyValue()
        {
            _destroyValueImpl<0, Ts...>(_getTypeIndex());
        }

        priv template <usize index, tname T, tname... Ts_>
        cexpr fn _destroyValueImpl(usize i)
        {
            using Types = TypeList<Ts_...>;

            if (i != index)
            {
                if cexpr (Types::Count == 0)
                {
                    TERMINATE("There is no type for current index.");
                }
                else
                {
                    // Recursion to find type at index i.
                    _destroyValueImpl<index + 1, Ts_...>(i);
                    return;
                }
            }

            ObjHelper().Destruct(&_getDataAs<T>());
        }

        prot template <tname T>
        cexpr fn _getDataAs() const -> const T&
        {
            return reinterpret_cast<const T&>(*_data.storage);
        }

        prot template <tname T>
        cexpr fn _getDataAs() -> T&
        {
            return reinterpret_cast<T&>(*_data.storage);
        }

    //// -------------------------------------------------------------------------------------------
    //// Fields
    //// -------------------------------------------------------------------------------------------

        priv AlignedUnionStorageFor<Ts...> _data;
        priv usize _index;
    };
}