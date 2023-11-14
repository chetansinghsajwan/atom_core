#pragma once
#include "Atom/Core/Primitives.h"
#include "Atom/TTI.h"

namespace Atom
{
    // clang-format off

    template <typename TPtr>
    concept RConstPtr = requires(TPtr ptr)
    {
        typename TPtr::TVal;

        { ptr.unwrap() } -> RConvertibleTo<const typename TPtr::TVal*>;
    };

    template <typename TPtr>
    concept RPtr = requires(TPtr ptr)
    {
        requires RConstPtr<TPtr>;

        { ptr.unwrap() } -> RSameAs<typename TPtr::TVal*>;
    };

    template <typename TPtr, typename T>
    concept RPtrOf = requires(TPtr ptr)
    {
        requires RPtr<TPtr>;
        requires RSameAs<T, typename TPtr::TVal>;
    };

    // clang-format on

    template <typename T>
    class Ptr;

    template <>
    class Ptr<void>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TVal_>
    class ConstPtr
    {
        static_assert(TTI::IsPure<TVal_>, "ConstPtr only supports pure types.");
        static_assert(not TTI::IsVoid<TVal_>, "ConstPtr doesn't support void.");

    private:
        using This = ConstPtr<TVal_>;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        using TVal = TVal_;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr ConstPtr():
            _ptr{ nullptr }
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr ConstPtr(const This&) = default;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr ConstPtr& operator=(const This&) = default;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr ConstPtr(This&&) = delete;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr ConstPtr& operator=(This&&) = delete;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr ConstPtr(std::nullptr_t):
            _ptr{ nullptr }
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr ConstPtr& operator=(std::nullptr_t)
        {
            _ptr = nullptr;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TPtr>
        constexpr ConstPtr(const TPtr& ptr)
            requires(RConstPtr<TPtr>) and (RConvertibleTo<const typename TPtr::TVal*, const TVal*>)
            :
            _ptr{ static_cast<const TVal*>(ptr.unwrap()) }
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TPtr>
        constexpr ConstPtr& operator=(const TPtr& ptr)
            requires(RConstPtr<TPtr>) and (RConvertibleTo<const typename TPtr::TVal*, const TVal*>)
        {
            _ptr = static_cast<const TVal*>(ptr.unwrap());
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr ConstPtr(const TVal* ptr):
            _ptr{ ptr }
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr ConstPtr& operator=(const TVal* ptr)
        {
            _ptr = ptr;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr ~ConstPtr() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val() const -> const TVal&
        {
            debug_expects(not isNull(), "Null ptr access.");

            return *_ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto checkVal() const -> const TVal&
        {
            expects(not isNull(), "Null ptr access.");

            return *_ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() const -> const TVal*
        {
            debug_expects(not isNull(), "Null ptr access.");

            return _ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto as() const -> ConstPtr<T>
        {
            return static_cast<const T*>(_ptr);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto unsafeAs() const -> ConstPtr<T>
        {
            return reinterpret_cast<const T*>(_ptr);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto dynAs() const -> ConstPtr<T>
        {
            return dynamic_cast<const T*>(_ptr);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto asMut() const -> Ptr<TVal>
        {
            return const_cast<TVal*>(_ptr);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto isNull() const -> bool
        {
            return unwrap() == nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto eq(std::nullptr_t) const -> bool
        {
            return unwrap() == nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto ne(std::nullptr_t) const -> bool
        {
            return not eq(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto eq(const This& that) const -> bool
        {
            return unwrap() == that.unwrap();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto ne(const This& that) const -> bool
        {
            return not eq(that);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto unwrap() const -> const TVal*
        {
            return _ptr;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # To Do
        ///
        /// - May be we should remove this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() const -> const TVal&
        {
            return val();
        }

    protected:
        const TVal* _ptr;
    };

    template <typename T>
    ConstPtr(const T* ptr) -> ConstPtr<T>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <>
    class ConstPtr<void>
    {
        using This = ConstPtr<void>;

    public:
        using TVal = void;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ConstPtr():
            _ptr{ nullptr }
        {}

        constexpr ConstPtr(const This&) = default;
        constexpr ConstPtr& operator=(const This&) = default;

        constexpr ConstPtr(This&&) = default;
        constexpr ConstPtr& operator=(This&&) = default;

        constexpr ConstPtr(std::nullptr_t):
            _ptr{ nullptr }
        {}

        constexpr ConstPtr& operator=(std::nullptr_t)
        {
            _ptr = nullptr;
            return *this;
        }

        template <typename TPtr>
        constexpr ConstPtr(const TPtr& ptr)
            requires(RConstPtr<TPtr>)
            :
            _ptr{ static_cast<const void*>(ptr.unwrap()) }
        {}

        template <typename TPtr>
        constexpr ConstPtr& operator=(const TPtr& ptr)
            requires(RConstPtr<TPtr>)
        {
            _ptr = static_cast<const void*>(ptr.unwrap());
            return *this;
        }

        template <typename T>
        constexpr ConstPtr(const T* ptr):
            _ptr{ static_cast<const void*>(ptr) }
        {}

        template <typename T>
        constexpr ConstPtr& operator=(const T* ptr)
        {
            _ptr = static_cast<const void*>(ptr);
            return *this;
        }

        constexpr ~ConstPtr() = default;

    public:
        constexpr auto unwrap() const -> const void*
        {
            return _ptr;
        }

        template <typename T>
        constexpr auto unwrapAs() const -> const byte*
        {
            return static_cast<const T*>(_ptr);
        }

        constexpr auto unwrapAsByte() const -> const byte*
        {
            return static_cast<const byte*>(_ptr);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto as() const -> ConstPtr<T>
        {
            return static_cast<const T*>(_ptr);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        // constexpr auto asMut() const -> Ptr<void>
        // {
        //     return const_cast<void*>(_ptr);
        // }

    protected:
        const void* _ptr;
    };

    ConstPtr(const void*) -> ConstPtr<void>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TVal_>
    class Ptr: public ConstPtr<TVal_>
    {
        static_assert(TTI::IsPure<TVal_>, "Ptr only supports pure types.");
        static_assert(not TTI::IsVoid<TVal_>, "Ptr doesn't support void.");

    private:
        using This = Ptr<TVal_>;
        using Base = ConstPtr<TVal_>;

    public:
        using TVal = Base::TVal;

    public:
        constexpr Ptr() = default;

        constexpr Ptr(const Ptr&) = default;
        constexpr Ptr& operator=(const Ptr&) = default;

        constexpr Ptr(Ptr&&) = default;
        constexpr Ptr& operator=(Ptr&&) = default;

        constexpr Ptr(std::nullptr_t):
            Base(nullptr)
        {}

        constexpr Ptr& operator=(std::nullptr_t)
        {
            _ptr = nullptr;
            return *this;
        }

        template <typename TPtr>
        constexpr Ptr(TPtr& ptr)
            requires(RPtr<TPtr>) and (RConvertibleTo<typename TPtr::TVal*, TVal*>)
            :
            Base(static_cast<TVal*>(ptr.unwrap()))
        {}

        template <typename TPtr>
        constexpr Ptr(TPtr&& ptr)
            requires(RPtr<TPtr>) and (RConvertibleTo<typename TPtr::TVal*, TVal*>)
            :
            Base(static_cast<TVal*>(ptr.unwrap()))
        {}

        template <typename TPtr>
        constexpr Ptr& operator=(TPtr& ptr)
            requires(RPtr<TPtr>) and (RConvertibleTo<typename TPtr::TVal*, TVal*>)
        {
            _ptr = static_cast<TVal*>(ptr.unwrap());
            return *this;
        }

        constexpr Ptr(TVal* ptr):
            Base(ptr)
        {}

        constexpr Ptr& operator=(TVal* ptr)
        {
            _ptr = ptr;
            return *this;
        }

        constexpr ~Ptr() = default;

    public:
        constexpr auto mutVal() -> TVal&
        {
            return *_mutPtr();
        }

        constexpr auto operator*() -> TVal&
        {
            return *_mutPtr();
        }

        constexpr auto operator->() -> TVal*
        {
            return _mutPtr();
        }

        constexpr auto unwrap() const -> TVal*
        {
            return _mutPtr();
        }

    private:
        constexpr auto _mutPtr() const -> TVal*
        {
            return const_cast<TVal*>(_ptr);
        }

    protected:
        using Base::_ptr;
    };

    template <typename T>
    Ptr(T* ptr) -> Ptr<T>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <>
    class Ptr<void>: public ConstPtr<void>
    {
        using This = Ptr<void>;
        using Base = ConstPtr<void>;

    public:
        constexpr Ptr() = default;

        constexpr Ptr(const Ptr&) = default;
        constexpr Ptr& operator=(const Ptr&) = default;

        constexpr Ptr(Ptr&&) = default;
        constexpr Ptr& operator=(Ptr&&) = default;

        constexpr Ptr(std::nullptr_t):
            Base(nullptr)
        {}

        constexpr Ptr& operator=(std::nullptr_t)
        {
            _ptr = nullptr;
            return *this;
        }

        template <typename TPtr>
        constexpr Ptr(TPtr& ptr)
            requires(RPtr<TPtr>)
            :
            Base(static_cast<void*>(ptr.unwrap()))
        {}

        template <typename TPtr>
        constexpr Ptr(TPtr&& ptr)
            requires(RPtr<TPtr>)
            :
            Base(static_cast<void*>(ptr.unwrap()))
        {}

        template <typename TPtr>
        constexpr Ptr& operator=(TPtr& ptr)
            requires(RPtr<TPtr>)
        {
            _ptr = static_cast<void*>(ptr.unwrap());
            return *this;
        }

        template <typename T>
        constexpr Ptr(T* ptr)
            requires(not RConst<T>)
            :
            Base(static_cast<void*>(ptr))
        {}

        template <typename T>
        constexpr Ptr& operator=(T* ptr)
            requires(not RConst<T>)
        {
            _ptr = static_cast<void*>(ptr);
            return *this;
        }

        constexpr ~Ptr() = default;

    public:
        constexpr auto unwrap() const -> void*
        {
            return _mutPtr();
        }

        constexpr auto unwrapAsByte() const -> byte*
        {
            return static_cast<byte*>(_mutPtr());
        }

        template <typename T>
        constexpr auto as() const -> Ptr<T>
        {
            return static_cast<T*>(_mutPtr());
        }

        template <typename T>
        constexpr auto unsafeAs() -> Ptr<T>
        {
            return reinterpret_cast<T*>(_mutPtr());
        }

    private:
        constexpr auto _mutPtr() const -> void*
        {
            return const_cast<void*>(_ptr);
        }

    protected:
        using Base::_ptr;
    };

    Ptr(void*) -> Ptr<void>;
}
