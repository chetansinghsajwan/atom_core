#pragma once
#include "Atom/Contracts.h"
#include "Atom/Core.h"
#include "Atom/Memory/ObjHelper.h"

namespace Atom
{
    template <typename TVal>
    union _OptionStorage
    {
        class _Dummy
        {};

    public:
        constexpr _OptionStorage():
            _dummy{}
        {}

        constexpr _OptionStorage(const _OptionStorage&) = default;
        constexpr _OptionStorage& operator=(const _OptionStorage&) = default;

        constexpr _OptionStorage(_OptionStorage&&) = default;
        constexpr _OptionStorage& operator=(_OptionStorage&&) = default;

        template <typename... TArgs>
        constexpr _OptionStorage(TArgs&&... args):
            _value{ forward<TArgs>(args)... }
        {}

        constexpr ~_OptionStorage()
            requires(RTriviallyDestructible<TVal>)
        = default;

        constexpr ~_OptionStorage()
            requires(not RTriviallyDestructible<TVal>)
        {}

    public:
        constexpr auto getData() -> MutMemPtr<TVal>
        {
            return &_value;
        }

        constexpr auto getData() const -> MemPtr<TVal>
        {
            return &_value;
        }

    private:
        TVal _value;
        _Dummy _dummy;
    };

    template <typename TVal_>
    class _OptionImpl
    {
        using This = _OptionImpl<TVal_>;

        /// --------------------------------------------------------------------------------------------
        /// # To Do
        ///
        /// - Create `StaticAlignedStorageFor<TVal_>` to replace this.
        /// --------------------------------------------------------------------------------------------
        using _TStorage = _OptionStorage<TVal_>;

    public:
        using TVal = TVal_;

        class CtorDefault
        {};

        class CtorCopy
        {};

        class CtorMove
        {};

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get the default value of [`TVal`].
        /// ----------------------------------------------------------------------------------------
        static consteval auto GetDefault() -> TVal
        {
            return TVal();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Trivial Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _OptionImpl() = default;

        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _OptionImpl(CtorDefault):
            _isValue{ false }, _storage{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _OptionImpl(const This& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _OptionImpl(CtorCopy, const This& that):
            This(CtorDefault())
        {
            if (that._isValue)
            {
                _createValue(that._getValue());
                _isValue = true;
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr _OptionImpl& operator=(const This& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _OptionImpl(This&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _OptionImpl(CtorMove, This&& that):
            This()
        {
            if (that._isValue)
            {
                _createValue(mov(that._getMutValue()));
                _isValue = true;
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr _OptionImpl& operator=(This&& that) = default;

        template <typename... TArgs>
        /// ----------------------------------------------------------------------------------------
        /// # Value Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _OptionImpl(TArgs&&... args):
            _storage{ forward<TArgs>(args)... }, _isValue{ true }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~_OptionImpl() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Copies [`Option`] into this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto copy(const _OptionImpl& that)
        {
            _setValueFromOption<false>(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Moves [`Option`] into this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto move(_OptionImpl&& that)
        {
            _setValueFromOption<true>(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Swaps [`Option`] with `that`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto swap(_OptionImpl& that)
        {
            _swap(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroy current value if any.
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy()
        {
            if (_isValue)
            {
                _destroyValue();
                _isValue = false;
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroys current value if any and constructs new value wih `args`.
        /// ----------------------------------------------------------------------------------------
        template <typename... TArgs>
        constexpr auto emplaceValue(TArgs&&... args)
        {
            if (_isValue)
            {
                _destroyValue();
                _createValue(forward<TArgs>(args)...);
            }
            else
            {
                _createValue(forward<TArgs>(args)...);
                _isValue = true;
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// If this contains value, assigns new value to it.
        /// Else, constructs new value.
        /// ----------------------------------------------------------------------------------------
        template <typename T1>
        constexpr auto setValue(T1&& val)
        {
            if (not _isValue)
            {
                _createValue(forward<T1>(val));
                _isValue = true;
            }
            else
            {
                _setValue(forward<T1>(val));
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Get const ref to current value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getValue() const -> const TVal&
        {
            return _getValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get ref to current value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto getMutValue() -> TVal&
        {
            return _getMutValue();
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if this contains value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isValue() const -> bool
        {
            return _isValue;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if this doesn't contains value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isNull() const -> bool
        {
            return not _isValue;
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroys current value if any.
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroyValue()
        {
            if (_isValue)
            {
                _destroyValue();
                _isValue = false;
            }
        }

    private:
        template <bool move, typename TOpt>
        constexpr auto _setValueFromOption(TOpt&& that)
        {
            if (that._isValue)
            {
                if (_isValue)
                {
                    if constexpr (move)
                        _setValue(mov(that._getMutValue()));
                    else
                        _setValue(that._getValue());
                }
                else
                {
                    if constexpr (move)
                        _createValue(mov(that._getMutValue()));
                    else
                        _createValue(that._getValue());

                    _isValue = true;
                }
            }
            else
            {
                if (_isValue)
                {
                    _destroyValue();
                    _isValue = false;
                }
            }
        }

        template <typename TOpt>
        constexpr auto _copy(const TOpt& that)
        {
            if (that._isValue)
            {
                if (_isValue)
                {
                    _setValue(that._getValue());
                }
                else
                {
                    _createValue(that._getValue());
                    _isValue = true;
                }
            }
            else
            {
                if (_isValue)
                {
                    _destroyValue();
                    _isValue = false;
                }
            }
        }

        template <typename TOpt>
        constexpr auto _move(TOpt&& that)
        {
            if (that._isValue)
            {
                if (_isValue)
                {
                    _setValue(mov(that._getValue()));
                }
                else
                {
                    _createValue(mov(that._getValue()));
                    _isValue = true;
                }
            }
            else
            {
                if (_isValue)
                {
                    _destroyValue();
                    _isValue = false;
                }
            }
        }

        constexpr auto _swap(_OptionImpl& that)
        {
            if (that._isValue)
            {
                if (_isValue)
                {
                    _swapValue(that._getValue());
                }
                else
                {
                    _createValue(mov(that._getMutValue()));
                    _isValue = true;
                    that._isValue = false;
                }
            }
            else
            {
                if (_isValue)
                {
                    that._createValue(mov(_getMutValue()));
                    that._isValue = true;
                    _isValue = false;
                }
            }
        }

        template <typename... TArgs>
        constexpr auto _createValue(TArgs&&... args)
        {
            ObjHelper().ConstructAs<TVal>(_storage.getData(), forward<TArgs>(args)...);
        }

        template <typename TArg>
        constexpr auto _setValue(TArg&& val)
        {
            ObjHelper().AssignAs<TVal>(_storage.getData(), forward<TArg>(val));
        }

        constexpr auto _swapValue(TVal& that)
        {
            ObjHelper().Swap(_getMutValue(), that);
        }

        constexpr auto _getValue() const -> const TVal&
        {
            return _storage.getData().val();
        }

        constexpr auto _getMutValue() -> TVal&
        {
            return _storage.getData().mutVal();
        }

        constexpr auto _destroyValue()
        {
            ObjHelper().DestructAs<TVal>(_storage.getData());
        }

    private:
        bool _isValue;
        _TStorage _storage;
    };
}
