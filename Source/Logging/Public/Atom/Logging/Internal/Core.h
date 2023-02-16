#pragma once
#include <string>
#include <vector>
#include <string_view>
#include <unordered_map>
#include <chrono>
#include <memory>
#include "Mutex.h"

#define ATOM_CONFIG_DEBUG
#define FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#define FORWARD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

namespace Atom
{
    using byte = unsigned char;
    using String = std::string;
    using StringView = std::string_view;
    using TimePoint = std::chrono::system_clock::time_point;

    using Exception = std::exception;
    using InvalidOperationException = std::runtime_error;
    using InvalidArgumentException = std::runtime_error;
    using NullPointerException = std::runtime_error;
    using AssertionException = std::runtime_error;

    template <typename T>
    using DynamicArray = std::vector<T>;

    template <typename T>
    using SharedPtr = std::shared_ptr<T>;

    template <typename K, typename T>
    using UnorderedMap = std::unordered_map<K, T>;

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

    template <typename ExceptionType = AssertionException>
    void ASSERT(bool assertion, auto&&... args)
    {
        if (!assertion)
        {
            throw ExceptionType(FORWARD(args)...);
        }
    }

    template <typename ExceptionType = AssertionException>
    void DEBUG_ASSERT(bool assertion, auto&&... args)
    {
#ifdef ATOM_CONFIG_DEBUG
        ASSERT<ExceptionType>(assertion, FORWARD(args)...);
#endif
    }

    template <typename T>
    T&& MOVE(T& obj) noexcept
    {
        return std::move(obj);
    }
}

#define ATOM_FUNCTION __func__

#define ATOM_DEBUG_EXPECTS_MSG(assertion, msg) DEBUG_ASSERT<AssertionException>(assertion, \
    String(ATOM_FUNCTION) + ": [DEBUG] Expects {" #assertion "}. " msg)

#define ATOM_DEBUG_EXPECTS(assertion) ATOM_DEBUG_EXPECTS_MSG(assertion, "")