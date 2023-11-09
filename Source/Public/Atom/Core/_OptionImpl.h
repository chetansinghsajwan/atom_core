#pragma once
#include "Atom/Contracts.h"
#include "Atom/Core.h"
#include "Atom/Memory/ObjHelper.h"

namespace Atom
{
    template <typename TVal>
    union _OptionStorage
    {
    public:
        class NoInit
        {};

    public:
        _OptionStorage() {}

        _OptionStorage(NoInit):
            _dummy{}
        {}

        template <typename... TArgs>
        _OptionStorage(TArgs&&... args):
            _value{ forward<TArgs>(args)... }
        {}

        _OptionStorage(const _OptionStorage&) = default;

        _OptionStorage& operator=(const _OptionStorage&) = default;

        _OptionStorage(_OptionStorage&&) = default;

        _OptionStorage& operator=(_OptionStorage&&) = default;

        ~_OptionStorage()
            requires(RTriviallyDestructible<TVal>)
        = default;

        ~_OptionStorage()
            requires(not RTriviallyDestructible<TVal>)
        {}

    public:
        constexpr auto getData() -> MemPtr<TVal>
        {
            return &_value;
        }

        constexpr auto getData() const -> ConstMemPtr<TVal>
        {
            return &_value;
        }

    private:
        TVal _value;
        NoInit _dummy;
    };

    template <typename TVal>
    union _OptionStorage<MemPtr<TVal>>
    {
    public:
        class NoInit
        {};

    public:
        _OptionStorage() = default;

        _OptionStorage(NoInit) {}

        _OptionStorage(MemPtr<TVal> ptr):
            _ptr{ ptr }
        {}

    public:
        constexpr auto getData() -> MemPtr<TVal>*
        {
            return &_ptr;
        }

        constexpr auto getData() const -> ConstMemPtr<TVal>*
        {
            return &_ptr;
        }

    private:
        MemPtr<TVal> _ptr;
    };

    template <typename T>
    class _OptionImpl
    {
        using _Storage = _OptionStorage<T>;
        using _StorageCtorNoInit = _Storage::NoInit;
        using This = _OptionImpl<T>;

    public:
        using TVal = T;

    public:
        static consteval auto GetDefault() -> const TVal&
        {
            return TVal();
        }

    public:
        constexpr _OptionImpl():
            _storage{ _StorageCtorNoInit{} }, _isValue{ false }
        {}

        constexpr _OptionImpl(const This& that)
            requires(RTriviallyCopyConstructible<TVal>)
        = default;

        constexpr _OptionImpl(const This& that)
            requires(not RTriviallyCopyConstructible<TVal>)
            : This()
        {
            constructValueFromOption(that);
        }

        constexpr _OptionImpl& operator=(const This& that) = default;

        constexpr _OptionImpl(This&& that)
            requires(RTriviallyMoveConstructible<TVal>)
        = default;

        constexpr _OptionImpl(This&& that)
            requires(not RTriviallyMoveConstructible<TVal>)
            : This()
        {
            constructValueFromOption(mov(that));
        }

        constexpr _OptionImpl& operator=(This&& that) = default;

        template <typename... TArgs>
        constexpr _OptionImpl(TArgs&&... args):
            _storage{ forward<TArgs>(args)... }, _isValue{ true }
        {}

        constexpr ~_OptionImpl()
            requires(RTriviallyDestructible<TVal>)
        = default;

        constexpr ~_OptionImpl()
            requires(not RTriviallyDestructible<TVal>)
        {
            if (_isValue)
            {
                _destroyValue();
            }
        }

    public:
        constexpr auto constructValueFromOption(const _OptionImpl& opt)
        {
            _constructValueFromOption<false>(opt);
        }

        constexpr auto constructValueFromOption(_OptionImpl&& opt)
        {
            _constructValueFromOption<true>(opt);
        }

        constexpr auto assignValueFromOption(const _OptionImpl& opt)
        {
            _assignValueFromOption<false>(opt);
        }

        constexpr auto assignValueFromOption(_OptionImpl&& opt)
        {
            _assignValueFromOption<true>(opt);
        }

        constexpr auto swapValueFromOption(_OptionImpl& opt)
        {
            _swapValueFromOption(opt);
        }

        template <typename... TArgs>
        constexpr auto constructValue(TArgs&&... args)
        {
            debug_expects(not _isValue);

            _constructValue(forward<TArgs>(args)...);
            _isValue = true;
        }

        template <typename... TArgs>
        constexpr auto emplaceValue(TArgs&&... args)
        {
            if (_isValue)
            {
                _destroyValue();
                _constructValue(forward<TArgs>(args)...);
            }
            else
            {
                _constructValue(forward<TArgs>(args)...);
                _isValue = true;
            }
        }

        template <typename T1>
        constexpr auto assignValue(T1&& val)
        {
            _assignValue(forward<T1>(val));
            _isValue = true;
        }

        constexpr auto destroyValue()
        {
            debug_expects(_isValue);

            _destroyValue();
            _isValue = false;
        }

        constexpr auto destroyValueWithCheck()
        {
            if (_isValue)
            {
                _destroyValue();
                _isValue = false;
            }
        }

        constexpr auto getValue() -> TVal&
        {
            debug_expects(_isValue);

            return _getValue();
        }

        constexpr auto getValue() const -> const TVal&
        {
            debug_expects(_isValue);

            return _getValue();
        }

        constexpr auto isValue() const -> bool
        {
            return _isValue;
        }

    private:
        template <bool move>
        constexpr auto _constructValueFromOption(auto&& opt)
        {
            if (opt._isValue)
            {
                if constexpr (move)
                    _constructValue(mov(opt._getValue()));
                else
                    _constructValue(opt._getValue());

                _isValue = true;
            }
        }

        template <bool move>
        constexpr auto _assignValueFromOption(auto&& opt)
        {
            if (opt._isValue)
            {
                if (_isValue)
                {
                    if constexpr (move)
                        _assignValue(mov(opt._getValue()));
                    else
                        _assignValue(opt._getValue());
                }
                else
                {
                    if constexpr (move)
                        _constructValue(mov(opt._getValue()));
                    else
                        _constructValue(opt._getValue());

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

        constexpr auto _swapValueFromOption(_OptionImpl& opt)
        {
            if (opt._isValue)
            {
                if (_isValue)
                {
                    _swapValue(opt._getValue());
                }
                else
                {
                    _constructValue(mov(opt._getValue()));
                    _isValue = true;
                    opt._isValue = false;
                }
            }
            else
            {
                if (_isValue)
                {
                    opt._constructValue(mov(_getValue()));
                    opt._isValue = true;
                    _isValue = false;
                }
            }
        }

        template <typename... TArgs>
        constexpr auto _constructValue(TArgs&&... args)
        {
            ObjHelper().ConstructAs<TVal>(_storage.getData(), forward<TArgs>(args)...);
        }

        template <typename TArg>
        constexpr auto _assignValue(TArg&& val)
        {
            ObjHelper().AssignAs<TVal>(_storage.getData(), forward<TArg>(val));
        }

        constexpr auto _swapValue(TVal& that)
        {
            ObjHelper().Swap(_getValue(), that);
        }

        constexpr auto _destroyValue()
        {
            ObjHelper().DestructAs<TVal>(_storage.getData());
        }

        constexpr auto _getValue() -> TVal&
        {
            return *_storage.getData();
        }

        constexpr auto _getValue() const -> const TVal&
        {
            return *_storage.getData();
        }

    private:
        _Storage _storage;
        bool _isValue;
    };
}
