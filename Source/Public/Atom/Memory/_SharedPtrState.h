#pragma once
#include "Atom/Core.h"

namespace Atom
{
    template <typename T>
    class EboHelper
    {
    public:
        constexpr EboHelper(T val)
            : _val(mov(val)) {}

    public:
        constexpr auto get() const -> const T&
        {
            return _val;
        }

        constexpr auto getMut() -> T&
        {
            return _val;
        }

    private:
        T _val;
    };

    template <typename T>
        requires TTI::IsEmpty<T>
    class EboHelper<T>: private T
    {        
    public:
        constexpr EboHelper(T val)
            : T(mov(val)) {}

    public:
        constexpr auto get() const -> const T&
        {
            return static_cast<const T&>(*this);
        }

        constexpr auto getMut() -> T&
        {
            return static_cast<T&>(*this);
        }
    };

    class _ISharedPtrState
    {
    public:
        virtual auto destroy(MutPtr<void> ptr) -> void = 0;

        virtual auto deallocSelf() -> void = 0;

        constexpr auto increaseCount()
        {
            _count++;
        }

        constexpr auto decreaseCount()
        {
            _count--;
        }

        constexpr auto getCount() const -> usize
        {
            return _count;
        }

    private:
        usize _count;
    };

    template <typename TVal, typename TDestroyer, typename TAllocator>
    class _SharedPtrState
        : public _ISharedPtrState
        , private EboHelper<TDestroyer>
        , private EboHelper<TAllocator>
    {
    private:
        using TDestroyerHelper = EboHelper<TDestroyer>;
        using TAllocatorHelper = EboHelper<TAllocator>;

    public:
        constexpr _SharedPtrState(TDestroyer destroyer, TAllocator allocator)
            : TDestroyerHelper(mov(destroyer))
            , TAllocatorHelper(mov(allocator))
        {}

    public:
        virtual auto destroy(MutPtr<void> ptr) -> void override final
        {
            TDestroyerHelper::getMut()(ptr.template as<TVal>());
        }

        virtual auto deallocSelf() -> void override final
        {
            TAllocatorHelper::getMut().Dealloc(this);
        }
    };
}
