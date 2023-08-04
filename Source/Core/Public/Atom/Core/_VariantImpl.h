#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"
#include "Atom/Memory/ObjHelper.h"
#include "TypeList.h"
#include "Storage.h"

namespace Atom
{
    template <tname... Ts>
    class _VariantStorage
    {
    public:
        constexpr fn getData() -> void*
        {
            return &_storage.storage;
        }

        constexpr fn getData() const -> const void*
        {
            return &_storage.storage;
        }

    private:
        AlignedUnionStorageFor<Ts...> _storage;
    };

    /// --------------------------------------------------------------------------------------------
    /// Implementatiion of [`Variant`].
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    class _VariantImpl
    {
    private:
        template <tname... TOthers>
        friend class _VariantImpl;

    private:
        using _Types = TypeList<Ts...>;
        using _Storage = _VariantStorage<Ts...>;

    //// -------------------------------------------------------------------------------------------
    //// Static Functions
    //// -------------------------------------------------------------------------------------------

    public:
        static cexpr fn GetTypeCount() -> usize
        {
            return _Types::Count;
        }

        template <tname T>
        static ceval fn HasType() -> bool
        {
            return _Types::template Has<T>;
        }

        template <usize i>
        static ceval fn HasIndex() -> bool
        {
            return i < _Types::Count;
        }

        template <tname T>
        static ceval fn GetIndexForType() -> usize
        {
            return _Types::template IndexOf<T>;
        }

        static ceval fn GetNullTypeIndex() -> usize
        {
            return -1;
        }

        template <usize i>
        using TypeAtIndex = tname _Types::template At<i>;

    //// -------------------------------------------------------------------------------------------
    //// Funcions
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Copy or move constructs value hold by variant `that`.
        /// 
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        template <tname... TOthers>
        cexpr fn constructValueFromVariant(const _VariantImpl<TOthers...>& that)
        {
            _constructValueFromVariantImpl<false, 0, TOthers...>(that, that.getTypeIndex());
        }

        template <tname... TOthers>
        cexpr fn constructValueFromVariant(_VariantImpl<TOthers...>&& that)
        {
            _constructValueFromVariantImpl<true, 0, TOthers...>(that, that.getTypeIndex());
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs or assigns value from `that` variant.
        /// Assigns if `that` variant holds the same type else constructs.
        /// ----------------------------------------------------------------------------------------
        template <tname... TOthers>
        cexpr fn setValueFromVariant(const _VariantImpl<TOthers...>& that)
        {
            _setValueFromVariantImpl<false, 0, TOthers...>(that, that.getTypeIndex());
        }

        template <tname... TOthers>
        cexpr fn setValueFromVariant(_VariantImpl<TOthers...>&& that)
        {
            _setValueFromVariantImpl<true, 0, TOthers...>(that, that.getTypeIndex());
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs value of type `T` with args `args`.
        /// 
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        template <tname T, tname... TArgs>
        cexpr fn constructValueByType(TArgs&&... args)
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
        template <usize i, tname... TArgs>
        cexpr fn constructValueByIndex(TArgs&&... args)
        {
            constructValueByType<TypeAtIndex<i>>(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs value of type `T` with args `args`.
        /// 
        /// # Expects
        /// - Current value is null.
        /// ----------------------------------------------------------------------------------------
        template <tname T, tname... TArgs>
        cexpr fn emplaceValueByType(TArgs&&... args)
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
        template <usize i, tname... TArgs>
        cexpr fn emplaceValueByIndex(TArgs&&... args)
        {
            emplaceValueByType<TypeAtIndex<i>>(fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs or assigns value of type deduced by `value` with `value`.
        /// ----------------------------------------------------------------------------------------
        template <tname TFwd>
        cexpr fn setValue(TFwd&& value)
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

        template <tname T>
        cexpr fn getValueByType() const -> const T&
        {
            debug_expects(GetIndexForType<T>() == getTypeIndex(),
                "Current type is not same as requested type.");

            return _getValueAs<T>();
        }

        template <tname T>
        cexpr fn getValueByType() -> T&
        {
            debug_expects(GetIndexForType<T>() == getTypeIndex(),
                "Current type is not same as requested type.");

            return _getValueAs<T>();
        }

        template <usize i>
        cexpr fn getValueByIndex() const -> const TypeAtIndex<i>&
        {
            return getValueByType<TypeAtIndex<i>>();
        }

        template <usize i>
        cexpr fn getValueByIndex() -> TypeAtIndex<i>&
        {
            return getValueByType<TypeAtIndex<i>>();
        }

        template <tname T>
        cexpr fn isType() const -> bool
        {
            return GetIndexForType<T>() == getTypeIndex();
        }

        template <usize i>
        cexpr fn isIndex() const -> bool
        {
            return i == getTypeIndex();
        }

        cexpr fn getTypeIndex() const -> usize
        {
            return _index;
        }

        cexpr fn destroyValue()
        {
            _destroyValueImpl<0, Ts...>(getTypeIndex());
        }

    private:
        template <bool move, usize index, tname TOther, tname... TOthers>
        cexpr fn _constructValueFromVariantImpl(auto& that, usize thatIndex)
        {
            using ThatTypes = TypeList<TOthers...>;

            if (index != thatIndex)
            {
                if cexpr (ThatTypes::Count == 0)
                {
                    panic("There is no type for current index.");
                }
                else
                {
                    _constructValueFromVariantImpl<move, index + 1, TOthers...>(that, thatIndex);
                    return;
                }
            }

            if cexpr (move)
            {
                _constructValueAs<TOther>(mov(that.template _getValueAs<TOther>()));
            }
            else
            {
                _constructValueAs<TOther>(that.template _getValueAs<TOther>());
            }

            _index = GetIndexForType<TOther>();
        }

        template <bool move, usize index, tname TOther, tname... TOthers>
        cexpr fn _setValueFromVariantImpl(auto&& that, usize thatIndex)
        {
            using ThatTypes = TypeList<TOthers...>;

            if (index != thatIndex)
            {
                if cexpr (ThatTypes::Count == 0)
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
                if cexpr (move)
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

                if cexpr (move)
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

        template <usize index, tname T, tname... Ts_>
        cexpr fn _destroyValueImpl(usize i)
        {
            using Types = TypeList<Ts_...>;

            if (i != index)
            {
                if cexpr (Types::Count == 0)
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
        cexpr fn _constructValueAs(auto&&... args)
        {
            ObjHelper().Construct(_getDataAs<T>(), fwd(args)...);
        }

        template <typename T>
        cexpr fn _assignValueAs(auto&& val)
        {
            ObjHelper().Assign(_getDataAs<T>(), fwd(val));
        }

        template <typename T>
        cexpr fn _destructValueAs()
        {
            ObjHelper().Destruct(_getDataAs<T>());
        }

        template <tname T>
        cexpr fn _getValueAs() -> T&
        {
            return *_getDataAs<T>();
        }

        template <tname T>
        cexpr fn _getValueAs() const -> const T&
        {
            return *_getDataAs<T>();
        }

        template <tname T>
        cexpr fn _getDataAs() -> T*
        {
            return reinterpret_cast<T*>(_storage.getData());
        }

        template <tname T>
        cexpr fn _getDataAs() const -> const T*
        {
            return reinterpret_cast<const T*>(_storage.getData());
        }

    //// -------------------------------------------------------------------------------------------
    //// Fields
    //// -------------------------------------------------------------------------------------------

    private:
        _Storage _storage;
        usize _index = 0;
    };
}