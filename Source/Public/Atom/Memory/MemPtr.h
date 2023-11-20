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
        template <typename T>
        constexpr MemPtrFunctions(Ptr<T> that):
            Base(that) {}

        template <typename T>
        constexpr MemPtrFunctions(MutPtr<T> that):
            Base(that) {}

        using Base::Base;
        using Base::operator=;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto next() const -> TThis
        {
            return _Make(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev() const -> TThis
        {
            return _Make(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto next(usize i) const -> TThis
        {
            return _Make(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto prev(usize i) const -> TThis
        {
            return _Make(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto move(isize i) const -> TThis
        {
            return _Make(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto move(usize i) const -> TThis
        {
            return _Make(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub(This that) const -> TThis
        {
            return _Make(_ptr - that.unwrap());
        }

    public:
        constexpr auto operator[](isize n) const
        {
            return Base::unwrap()[n.unwrap()];
        }

        constexpr auto operator[](usize n) const
        {
            return Base::unwrap()[n.unwrap()];
        }

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
            return _mutThis();
        }

        template <typename TInt>
        constexpr auto operator-=(TInt n) -> TThis&
            requires RInt<TInt> or _RInt<TInt>
        {
            _ptr = _Arithmetic(_ptr) - _UnwrapInt(n);
            return _mutThis();
        }

        constexpr auto operator-(This ptr) const -> isize
        {
            return _Arithmetic(_ptr) - _Arithmetic(ptr._ptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto gt(This ptr) const -> bool
        {
            return _ptr > ptr._ptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto ge(This ptr) const -> bool
        {
            return _ptr >= ptr._ptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto lt(This ptr) const -> bool
        {
            return _ptr < ptr._ptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto le(This ptr) const -> bool
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

        constexpr auto _mutThis() -> TThis&
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
    class MemPtr: public MemPtrFunctions<Ptr<T>, MemPtr<T>>
    {
        using This = MemPtr<T>;
        using Base = MemPtrFunctions<Ptr<T>, This>;

    public:
        constexpr MemPtr(const Ptr<T>& that):
            Base(that) {}

        using Base::Base;
        using Base::operator=;
    };

    template <typename T>
    MemPtr(const T* ptr) -> MemPtr<T>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutMemPtr: public MemPtrFunctions<MutPtr<T>, MutMemPtr<T>>
    {
        using This = MutMemPtr<T>;
        using Base = MemPtrFunctions<MutPtr<T>, This>;

    public:
        constexpr MutMemPtr(const MutPtr<T>& that):
            Base(that) {}

        using Base::Base;
        using Base::operator=;
    };

    template <typename T>
    MutMemPtr(const T* ptr) -> MutMemPtr<T>;
}
