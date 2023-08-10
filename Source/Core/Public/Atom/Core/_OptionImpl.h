#pragma once
#include "Atom/Core.h"
#include "Atom/Memory/ObjHelper.h"
#include "Atom/Exceptions/Assertions.h"

namespace Atom
{
    template <typename T>
    union _OptionStorage
    {
    public:
        class NoInit{};

    public:
        _OptionStorage() { }

        _OptionStorage(NoInit):
            _dummy{ } { }

        _OptionStorage(auto&&... args):
            _value{ fwd(args)... } { }

        _OptionStorage(const _OptionStorage&) = default;

        _OptionStorage& operator=(const _OptionStorage&) = default;

        _OptionStorage(_OptionStorage&&) = default;

        _OptionStorage& operator=(_OptionStorage&&) = default;

        ~_OptionStorage()
            requires(RTriviallyDestructible<T>)
            = default;

        ~_OptionStorage()
            requires(not RTriviallyDestructible<T>)
            { }

    public:
        constexpr auto getData() -> T*
        {
            return &_value;
        }

        constexpr auto getData() const -> const T*
        {
            return &_value;
        }

    private:
        T _value;
        NoInit _dummy;
    };

    template <typename T>
    union _OptionStorage<T*>
    {
    public:
        class NoInit{};

    public:
        _OptionStorage() = default;

        _OptionStorage(NoInit) { }

        _OptionStorage(T* ptr):
            _ptr{ ptr } { }

    public:
        constexpr auto getData() -> T**
        {
            return &_ptr;
        }

        constexpr auto getData() const -> const T**
        {
            return &_ptr;
        }

    private:
        T* _ptr;
    };

    template <typename T>
    class _OptionImpl
    {
        using _Storage = _OptionStorage<T>;
        using _StorageCtorNoInit = _Storage::NoInit;

    public:
        class CtorNoVal{};

    public:
        static consteval auto GetDefault() -> const T&
        {
            return T();
        }

    public:
        constexpr _OptionImpl() = default;

        constexpr _OptionImpl(CtorNoVal):
            _storage{ _StorageCtorNoInit{} }, _isValue{ false } { }

        constexpr _OptionImpl(auto&&... args):
            _storage{ fwd(args)... }, _isValue{ true } { }

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

        constexpr auto constructValue(auto&&... args)
        {
            debug_expects(not _isValue);

            _constructValue(fwd(args)...);
            _isValue = true;
        }

        constexpr auto emplaceValue(auto&&... args)
        {
            if (_isValue)
            {
                _destroyValue();
                _constructValue(fwd(args)...);
            }
            else
            {
                _constructValue(fwd(args)...);
                _isValue = true;
            }
        }

        constexpr auto assignValue(auto&& val)
        {
            _assignValue(fwd(val));
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

        constexpr auto _constructValue(auto&&... args)
        {
            ObjHelper().Construct<T>(_storage.getData(), fwd(args)...);
        }

        constexpr auto _assignValue(auto&& val)
        {
            ObjHelper().Assign<T>(_storage.getData(), fwd(val));
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