#pragma once
#include "Atom/Core.h"
#include "Atom/Memory/ObjHelper.h"
#include "Atom/Memory/SharedPtr.decl.h"
#include "Atom/TTI.h"

namespace Atom
{
    template <typename T>
    class UniquePtrDefaultDestroyer
    {
        static_assert(TTI::IsPure<T>, "UniquePtrDefaultDestroyer only supports pure types.");
        static_assert(not TTI::IsVoid<T>, "UniquePtrDefaultDestroyer doesn't support void.");

    public:
        constexpr auto operator()(MutPtr<T> val)
        {
            ObjHelper().DestructAs<T>(val);
            DefaultMemAllocator().Dealloc(val);
        }
    };

    template <typename TInVal, typename TInDestroyer = UniquePtrDefaultDestroyer<TInVal>>
    class UniquePtr: public MutPtr<TInVal>
    {
        static_assert(TTI::IsPure<TInVal>, "UniquePtr only supports pure types.");
        static_assert(not TTI::IsVoid<TInVal>, "UniquePtr doesn't support void.");
        static_assert(TTI::IsPure<TInDestroyer>);
        static_assert(not TTI::IsVoid<TInDestroyer>);

    private:
        template <typename TVal1, typename TDestroyer1>
        friend class UniquePtr;

    private:
        using Base = MutPtr<TInVal>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Type of value `This` holds.
        /// ----------------------------------------------------------------------------------------
        using TVal = TInVal;

        /// ----------------------------------------------------------------------------------------
        /// Type of destroyer used to destroy value and dealloc memory.
        /// ----------------------------------------------------------------------------------------
        using TDestroyer = TInDestroyer;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr(TDestroyer destroyer = TDestroyer())
            : Base()
            , _destroyer(mov(destroyer))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr(const UniquePtr& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr& operator=(const UniquePtr& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr(UniquePtr&& that)
            : Base(that.unwrap())
            , _destroyer(mov(that._destroyer))
        {
            that._setPtr(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr UniquePtr& operator=(UniquePtr&& that)
        {
            _move(mov(that));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TVal1>
        constexpr UniquePtr(UniquePtr<TVal1, TDestroyer>&& that)
            requires RSameOrDerivedFrom<TVal1, TVal>
            : Base(that.unwrap())
            , _destroyer(mov(that._destroyer))
        {
            that._setPtr(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TVal1>
        constexpr UniquePtr& operator=(UniquePtr<TVal1, TDestroyer>&& that)
            requires RSameOrDerivedFrom<TVal1, TVal>
        {
            _move(mov(that));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr explicit UniquePtr(MutPtr<TVal> ptr, TDestroyer destroyer = TDestroyer())
            : Base(ptr)
            , _destroyer(mov(destroyer))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~UniquePtr()
        {
            _checkAndDestroyValue();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(MutPtr<TVal> ptr)
        {
            _checkAndDestroyValue();

            _setPtr(ptr.unwrap());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(MutPtr<TVal> ptr, TDestroyer destroyer = TDestroyer())
        {
            _checkAndDestroyValue();

            _setPtr(ptr.unwrap());
            _destroyer = mov(destroyer);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(std::nullptr_t)
        {
            _checkAndDestroyValue();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto release() -> MutPtr<TVal>
        {
            return _releaseValue();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy()
        {
            _checkAndDestroyValue();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T = TVal>
        constexpr auto toShared() -> SharedPtr<T>
            requires RSameOrDerivedFrom<T, TVal>
        {
            return _toShared();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TAllocator, typename T = TVal>
        constexpr auto toSharedWithAlloc(TAllocator allocator = TAllocator()) -> SharedPtr<T>
            requires RSameOrDerivedFrom<T, TVal>
        {
            return _toShared();
        }

    private:
        template <typename TVal1>
        constexpr auto _move(UniquePtr<TVal1, TDestroyer>&& that)
        {
            _checkAndDestroyValue();

            _setPtr(that._getMutPtr());
            _destroyer = mov(that._destroyer);
            that._setPtr(nullptr);
        }

        constexpr auto _checkAndDestroyValue()
        {
            if (_getPtr() != nullptr)
            {
                _destroyValue();
                _setPtr(nullptr);
            }
        }

        constexpr auto _releaseValue() -> TVal*
        {
            TVal* ptr = _getMutPtr();
            _setPtr(nullptr);
            return ptr;
        }

        constexpr auto _destroyValue()
        {
            _destroyer(_getMutPtr());
        }

        constexpr auto _getPtr() const -> const TVal*
        {
            return Base::unwrap();
        }

        constexpr auto _getMutPtr() -> TVal*
        {
            return Base::unwrap();
        }

        constexpr auto _setPtr(TVal* ptr)
        {
            return Base::set(ptr);
        }

        template <typename TAllocator, typename TVal1>
        constexpr auto _toShared(TAllocator allocator) -> SharedPtr<TVal1>;

    private:
        TDestroyer _destroyer;
    };

    template <typename T, typename... TArgs>
    auto MakeUnique(TArgs&&... args) -> UniquePtr<T>
    {
        return std::make_unique<T>(forward<TArgs>(args)...);
    }

    template <typename T, typename TAllocator, typename... TArgs>
    auto MakeUniqueWithAlloc(TAllocator allocator, TArgs&&... args) -> UniquePtr<T>
    {
        return std::make_unique<T>(forward<TArgs>(args)...);
    }
}
