#pragma once
#include <iostream>

namespace Atom
{
    class BuildConfig
    {
    private:
        enum class _Config
        {
            Debug,
            Release
        };

    private:
        static consteval auto _GetConfig() -> _Config
        {
#ifdef NDEBUG
            return _Config::Release;
#else
            return _Config::Debug;
#endif
        }

    public:
        static consteval auto IsDebug() -> bool
        {
            return _GetConfig() == _Config::Debug;
        }

        static consteval auto IsRelease() -> bool
        {
            return _GetConfig() == _Config::Debug;
        }
    };

    template <typename T>
    constexpr auto mov(T&& t) -> typename std::remove_reference<T>::type&&
    {
        return static_cast<typename std::remove_reference<T>::type&&>(t);
    }

    template <typename T>
    constexpr T&& forward(typename std::remove_reference<T>::type& t)
    {
        return static_cast<T&&>(t);
    }

    template <typename T>
    constexpr T&& forward(typename std::remove_reference<T>::type&& t)
    {
        static_assert(
            !std::is_lvalue_reference<T>::value, "Can not forward an rvalue as an lvalue.");

        return static_cast<T&&>(t);
    }
}
