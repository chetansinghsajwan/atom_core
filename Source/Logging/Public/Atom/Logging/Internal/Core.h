#pragma once
#include <string>
#include <vector>
#include <string_view>
#include <chrono>
#include <memory>

namespace Atom
{
    using byte = unsigned char;
    using String = std::string;
    using StringView = std::string_view;
    using TimePoint = std::chrono::system_clock::time_point;

    namespace Time
    {
        auto Now()
        {
            return std::chrono::system_clock::now();
        }
    }

    template <typename T>
    concept LogStringType = requires(T a)
    {
        { std::hash<T>{}(a) } -> std::convertible_to<StringView>;
    };

    template <typename T>
    concept LogArgType = requires(T a)
    {
        { std::hash<T>{}(a) } -> std::convertible_to<StringView>;
    };

    template <typename T>
    using DynamicArray = std::vector<T>;

    template <typename T>
    using SharedPtr = std::shared_ptr<T>;
}

#define FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)