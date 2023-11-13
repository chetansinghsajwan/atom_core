#pragma once
#include "Atom/Memory/Ptr.h"
#include "Atom/Contracts.h"

namespace Atom
{
    using memunit = byte;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TBase, typename TThis>
    class MemPtrFunctions: public TBase
    {
        using This = MemPtrFunctions<TBase, TThis>;
        using Base = TBase;

    public:
        using TVal = typename Base::TVal;

    public:
        using Base::Base;
        using Base::operator=;

    public:
        template <typename TInt>
        constexpr auto operator+(TInt n) const -> TThis
            requires RInt<TInt> or _RInt<TInt>
        {
            return _Make(_Arithmetic(_ptr) + _UnwrapInt(n));
        }

        template <typename TInt>
        constexpr auto operator-(TInt n) const -> TThis
            requires RInt<TInt> or _RInt<TInt>
        {
            return _Make(_Arithmetic(_ptr) - _UnwrapInt(n));
        }

        constexpr auto operator++(int) -> TThis
        {
            return _Make(_Arithmetic(_ptr) + 1);
        }

        constexpr auto operator--(int) -> TThis
        {
            return _Make(_Arithmetic(_ptr) - 1);
        }

        template <typename TInt>
        constexpr auto operator+=(TInt n) -> TThis&
            requires RInt<TInt> or _RInt<TInt>
        {
            _ptr = _Arithmetic(_ptr) + _UnwrapInt(n);
            return _this();
        }

        template <typename TInt>
        constexpr auto operator-=(TInt n) -> TThis&
            requires RInt<TInt> or _RInt<TInt>
        {
            _ptr = _Arithmetic(_ptr) - _UnwrapInt(n);
            return _this();
        }

        constexpr auto operator-(This ptr) const -> isize
        {
            return _Arithmetic(_ptr) - _Arithmetic(ptr._ptr);
        }

        constexpr auto operator==(std::nullptr_t) const -> bool
        {
            return _ptr == nullptr;
        }

        constexpr auto operator!=(std::nullptr_t) const -> bool
        {
            return _ptr != nullptr;
        }

        constexpr auto operator==(This ptr) const -> bool
        {
            return _ptr == ptr._ptr;
        }

        constexpr auto operator!=(This ptr) const -> bool
        {
            return _ptr != ptr._ptr;
        }

        constexpr auto operator>(This ptr) const -> bool
        {
            return _ptr > ptr._ptr;
        }

        constexpr auto operator>=(This ptr) const -> bool
        {
            return _ptr >= ptr._ptr;
        }

        constexpr auto operator<(This ptr) const -> bool
        {
            return _ptr < ptr._ptr;
        }

        constexpr auto operator<=(This ptr) const -> bool
        {
            return _ptr <= ptr._ptr;
        }

    private:
        static constexpr auto _Arithmetic(const TVal* ptr)
        {
            if constexpr (RSameAs<TVal, void>)
            {
                return static_cast<const byte*>(ptr);
            }
            else
            {
                return ptr;
            }
        }

        static constexpr auto _Arithmetic(TVal* ptr)
        {
            if constexpr (RSameAs<TVal, void>)
            {
                return static_cast<byte*>(ptr);
            }
            else
            {
                return ptr;
            }
        }

        static constexpr auto _Make(const TVal* ptr) -> TThis
        {
            return TThis(const_cast<TVal*>(ptr));
        }

        constexpr auto _this() const -> const TThis&
        {
            return static_cast<const TThis&>(*this);
        }

        constexpr auto _this() -> TThis&
        {
            return static_cast<TThis&>(*this);
        }

    protected:
        using Base::_ptr;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ConstMemPtr: public MemPtrFunctions<ConstPtr<T>, ConstMemPtr<T>>
    {
        using This = ConstMemPtr<T>;
        using Base = MemPtrFunctions<ConstPtr<T>, This>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    template <typename T>
    ConstMemPtr(const T* ptr) -> ConstMemPtr<T>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MemPtr: public MemPtrFunctions<Ptr<T>, MemPtr<T>>
    {
        using This = MemPtr<T>;
        using Base = MemPtrFunctions<Ptr<T>, This>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    template <typename T>
    MemPtr(const T* ptr) -> MemPtr<T>;
}
