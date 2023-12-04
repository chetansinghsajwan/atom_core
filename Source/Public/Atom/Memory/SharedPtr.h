#pragma once
#include "Atom/Memory/ObjHelper.h"
#include "Atom/Memory/Ptr.h"
#include "Atom/Memory/UniquePtr.h"
#include "Atom/Memory/_SharedPtrState.h"
#include "Atom/TTI.h"

namespace Atom
{
    class SharedPtrDefaultAllocator: public DefaultMemAllocator
    {};

    template <typename T>
    class SharedPtrDefaultDestroyer
    {
        static_assert(TTI::IsPure<T>, "SharedPtrDefaultDestroyer only supports pure types.");
        static_assert(not TTI::IsVoid<T>, "SharedPtrDefaultDestroyer doesn't support void.");

    public:
        constexpr auto operator()(MutPtr<T> val)
        {
            ObjHelper().DestructAs<T>(val);
            DefaultMemAllocator().Dealloc(val);
        }
    };

    template <typename TInVal>
    class SharedPtr: public MutPtr<TInVal>
    {
        static_assert(TTI::IsPure<TInVal>, "SharedPtr only supports pure types.");
        static_assert(not TTI::IsVoid<TInVal>, "SharedPtr doesn't support void.");

    private:
        using This = SharedPtr<TInVal>;
        using Base = MutPtr<TInVal>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Type of value `This` holds.
        /// ----------------------------------------------------------------------------------------
        using TVal = typename Base::TVal;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr()
            : Base()
            , _state(nullptr)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr(const SharedPtr& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr SharedPtr(const SharedPtr<TThat>& that)
            requires RSameOrDerivedFrom<TThat, TVal>
            : Base(that.unwrap())
            , _state(that._state)
        {
            _checkAndIncreaseSharedCount();
        }

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr& operator=(const SharedPtr& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TVal1>
        constexpr SharedPtr& operator=(const SharedPtr<TVal1>& that)
            requires RSameOrDerivedFrom<TVal1, TVal>
        {
            _checkAndRelease();

            _setPtr(that._getMutPtr());
            _state = that._state;

            _checkAndIncreaseSharedCount();

            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr(SharedPtr&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr& operator=(SharedPtr&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr SharedPtr(SharedPtr<TThat>&& that)
            requires RSameOrDerivedFrom<TThat, TVal>
            : Base(that.unwrap())
            , _state(that._state)
        {
            that._setPtr(nullptr);
            that._state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Operator
        /// ----------------------------------------------------------------------------------------
        template <typename TThat>
        constexpr SharedPtr& operator=(SharedPtr<TThat>&& that)
            requires RSameOrDerivedFrom<TThat, TVal>
        {
            _checkAndRelease();

            _setPtr(that._getMutPtr());
            _state = that._state;

            that._setPtr(nullptr);
            that._state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Null Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr(std::nullptr_t)
            : This()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # Null Operator
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr& operator=(std::nullptr_t)
        {
            _checkAndRelease();

            _setPtr(nullptr);
            _state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename TDestroyer, typename TAllocator>
        constexpr SharedPtr(MutPtr<TVal> ptr,
            TDestroyer destroyer = SharedPtrDefaultDestroyer<TVal>(),
            TAllocator allocator = SharedPtrDefaultAllocator())
            : Base(ptr)
        {
            if (ptr.ne(nullptr))
            {
                _state = _createState<TDestroyer, TAllocator>(mov(destroyer), mov(allocator));
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Operator
        /// ----------------------------------------------------------------------------------------
        constexpr SharedPtr& operator=(MutPtr<TVal> ptr)
        {
            set(ptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~SharedPtr()
        {
            _checkAndRelease();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T, typename TDestroyer, typename TAllocator>
        constexpr auto set(MutPtr<T> ptr, TDestroyer destroyer = SharedPtrDefaultDestroyer<TVal>(),
            TAllocator allocator = SharedPtrDefaultAllocator())
            requires RSameOrDerivedFrom<T, TVal>
        {
            if (_getPtr() != nullptr)
            {
                _release();

                if (ptr != nullptr)
                {
                    _setPtr(ptr);
                    _createState(mov(destroyer), mov(allocator));
                }
                else
                {
                    _setPtr(nullptr);
                    _state = nullptr;
                }
            }
            else
            {
                if (ptr != nullptr)
                {
                    _setPtr(ptr);
                    _createState(mov(destroyer), mov(allocator));
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(std::nullptr_t)
        {
            _checkAndRelease();

            _setPtr(nullptr);
            _state = nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto release() -> MutPtr<TVal>
        {
            _checkAndRelease();

            MutPtr<TVal> ptr = _getMutPtr();
            _setPtr(nullptr);
            _state = nullptr;

            return ptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto getCount() const -> usize
        {
            return _state.eq(nullptr) ? 0 : _state.get().getCount();
        }

    private:
        constexpr auto _release()
        {
            _state.getMut().decreaseCount();

            if (_state.getMut().getCount() == 0)
            {
                _state.getMut().destroy(_getMutPtr());
                _state.getMut().deallocSelf();
            }
        }

        constexpr auto _checkAndRelease()
        {
            if (_getPtr() != nullptr)
            {
                _release();
            }
        }

        template <typename TDestroyer, typename TAllocator>
        constexpr auto _createState(TDestroyer destroyer, TAllocator allocator)
            -> MutPtr<_ISharedPtrState>
        {
            using State = _SharedPtrState<TDestroyer, TAllocator>();

            MutPtr<void> mem = allocator.Alloc(sizeof(State));
            ObjHelper().ConstructAs<State>(mem, mov(destroyer), mov(allocator));
            return mem.as<State>();
        }

        constexpr auto _checkAndIncreaseSharedCount()
        {
            if (_state != nullptr)
            {
                _state.getMut().increaseCount();
            }
        }

        constexpr auto _setPtr(MutPtr<TVal> ptr)
        {
            Base::set(ptr);
        }

        constexpr auto _getPtr() const -> const TVal*
        {
            return Base::unwrap();
        }

        constexpr auto _getMutPtr() -> TVal*
        {
            return Base::unwrap();
        }

    private:
        MutPtr<_ISharedPtrState> _state;
    };

    template <typename T, typename... TArgs>
    auto MakeShared(TArgs&&... args) -> SharedPtr<T>
    {
        return std::make_shared<T>(forward<TArgs>(args)...);
    }

    template <typename T, typename TAllocator, typename... TArgs>
    auto MakeSharedWithAlloc(TAllocator allocator, TArgs&&... args) -> SharedPtr<T>
    {
        return std::make_shared<T>(forward<TArgs>(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TVal, typename TDestroyer>
    template <typename TAllocator, typename TVal1>
    constexpr auto UniquePtr<TVal, TDestroyer>::_toShared(TAllocator allocator) -> SharedPtr<TVal1>
    {
        return MakeSharedWithAlloc(mov(allocator), _getMutPtr());
    }
}
