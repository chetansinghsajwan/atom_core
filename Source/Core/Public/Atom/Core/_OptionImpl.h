#pragma once
#include "Atom/Core.h"
#include "Atom/Memory/ObjHelper.h"

#define debug_expects(assertion, msg) if (!(assertion)) throw 0;
#define expects(assertion, msg) if (!(assertion)) throw 0;

namespace Atom
{
    template <tname T>
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

        _OptionStorage& op=(const _OptionStorage&) = default;

        _OptionStorage(_OptionStorage&&) = default;

        _OptionStorage& op=(_OptionStorage&&) = default;

        ~_OptionStorage()
            requires(RTriviallyDestructible<T>)
            = default;

        ~_OptionStorage()
            requires(not RTriviallyDestructible<T>)
            { }

    public:
        constexpr fn getData() -> T*
        {
            return &_value;
        }

        constexpr fn getData() const -> const T*
        {
            return &_value;
        }

    private:
        T _value;
        NoInit _dummy;
    };

    /// --------------------------------------------------------------------------------------------
    /// # To Do
    /// 
    /// - Optimize operations.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class _OptionImpl
    {
        using _Storage = _OptionStorage<T>;
        using _StorageCtorNoInit = _Storage::NoInit;

    public:
        class CtorNoVal{};

    public:
        static consteval fn GetDefault() -> const T&
        {
            return T();
        }

    public:
        cexpr ctor _OptionImpl() = default;

        cexpr ctor _OptionImpl(CtorNoVal):
            _storage{ _StorageCtorNoInit{} }, _isValue{ false } { }

        cexpr ctor _OptionImpl(auto&&... args):
            _storage{ fwd(args)... }, _isValue{ true } { }

    public:
        constexpr fn copyConstructValueFromOption(const _OptionImpl& opt)
        {
            _constructValueFromOption<false>(opt);
        }

        constexpr fn moveConstructValueFromOption(_OptionImpl& opt)
        {
            _constructValueFromOption<true>(opt);
        }

        constexpr fn copyAssignValueFromOption(const _OptionImpl& opt)
        {
            _assignValueFromOption<false>(opt);
        }

        constexpr fn moveAssignValueFromOption(_OptionImpl&& opt)
        {
            _assignValueFromOption<true>(opt);
        }

        constexpr fn constructValue(auto&&... args)
        {            
            ObjHelper().Construct<T>(_storage.getData(), fwd(args)...);
            _isValue = true;
        }

        constexpr fn assignValue(auto&& val)
        {
            ObjHelper().Assign<T>(_storage.getData(), fwd(val));
            _isValue = true;
        }

        constexpr fn destroyValue()
        {
            if (isValue())
            {
                ObjHelper().Destruct<T>(_storage.getData());
                _isValue = false;
            }
        }

        constexpr fn getValue() -> T&
        {
            debug_expects(isValue(), "");

            return *_storage.getData();
        }

        constexpr fn getValue() const -> const T&
        {
            debug_expects(isValue(), "");

            return *_storage.getData();
        }

        constexpr fn isValue() const -> bool
        {
            return _isValue;
        }

    private:
        template <bool move>
        constexpr fn _constructValueFromOption(auto&& opt)
        {
            if (opt.isValue())
            {
                if constexpr (move)
                    constructValue(mov(opt.getValue()));
                else
                    constructValue(opt.getValue());
            }
        }

        template <bool move>
        constexpr fn _assignValueFromOption(auto&& opt)
        {
            if (opt.isValue())
            {
                if (isValue())
                {
                    if constexpr (move)
                        assignValue(mov(opt.getValue()));
                    else
                        assignValue(opt.getValue());
                }
                else
                {
                    if constexpr (move)
                        constructValue(mov(opt.getValue()));
                    else
                        constructValue(opt.getValue());
                }
            }
            else
            {
                destroyValue();
            }
        }

    private:
        _Storage _storage;
        bool _isValue;
    };
}