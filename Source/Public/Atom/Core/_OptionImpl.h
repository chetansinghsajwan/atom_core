#pragma once
#include "Atom/Contracts.h"
#include "Atom/Core.h"
#include "Atom/Memory/ObjHelper.h"

namespace Atom
{
    template <typename T>
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
            requires(RTriviallyDestructible<T>)
        = default;

        ~_OptionStorage()
            requires(not RTriviallyDestructible<T>)
        {}

    public:
        constexpr auto getData() -> MemPtr<T>
        {
            return &_value;
        }

        constexpr auto getData() const -> MemPtr<const T>
        {
            return &_value;
        }

    private:
        T _value;
        NoInit _dummy;
    };

    template <typename T>
    union _OptionStorage<MemPtr<T>>
    {
    public:
        class NoInit
        {};

    public:
        _OptionStorage() = default;

        _OptionStorage(NoInit) {}

        _OptionStorage(MemPtr<T> ptr):
            _ptr{ ptr }
        {}

    public:
        constexpr auto getData() -> MemPtr<T>*
        {
            return &_ptr;
        }

        constexpr auto getData() const -> MemPtr<const T>*
        {
            return &_ptr;
        }

    private:
        MemPtr<T> _ptr;
    };

    template <typename T>
    class _OptionImpl
    {
        using _Storage = _OptionStorage<T>;
        using _StorageCtorNoInit = _Storage::NoInit;

    public:
        class CtorNoVal
        {};

    public:
        static consteval auto GetDefault() -> const T&
        {
            return T();
        }

    public:
        constexpr _OptionImpl() = default;

        constexpr _OptionImpl(CtorNoVal):
            _storage{ _StorageCtorNoInit{} }, _isValue{ false }
        {}

        template <typename... TArgs>
        constexpr _OptionImpl(TArgs&&... args):
            _storage{ forward<TArgs>(args)... }, _isValue{ true }
        {}

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

        constexpr auto destroyValueOnDestructor()
        {
            if (_isValue)
            {
                _destroyValue();
            }
        }

        constexpr auto getValue() -> T&
        {
            debug_expects(_isValue);

            return _getValue();
        }

        constexpr auto getValue() const -> const T&
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
            ObjHelper().Construct<T>(_storage.getData(), forward<TArgs>(args)...);
        }

        template <typename TArg>
        constexpr auto _assignValue(TArg&& val)
        {
            ObjHelper().Assign<T>(_storage.getData(), forward<TArg>(val));
        }

        constexpr auto _swapValue(T& that)
        {
            ObjHelper().Swap(_getValue(), that);
        }

        constexpr auto _destroyValue()
        {
            ObjHelper().Destruct<T>(_storage.getData());
        }

        constexpr auto _getValue() -> T&
        {
            return *_storage.getData();
        }

        constexpr auto _getValue() const -> const T&
        {
            return *_storage.getData();
        }

    private:
        _Storage _storage;
        bool _isValue;
    };
}
