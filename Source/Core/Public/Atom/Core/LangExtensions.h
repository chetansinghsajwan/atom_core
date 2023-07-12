#pragma once

#define ATOM_IS_CONFIG_DEBUG true
#define ATOM_IF_DEBUG if constexpr (ATOM_IS_CONFIG_DEBUG)

#define ATOM_FUNCTION __func__

#define tname typename
#define noex noexcept
#define noex_if(...) noex(noex(__VA_ARGS__))
#define is_noex(...) noex(__VA_ARGS__)
#define ofinal override final
#define fwd(...) static_cast<decltype(__VA_ARGS__)>(__VA_ARGS__)
#define declval(T) std::declval<T>()
#define noret [[noreturn]]
#define ctor
#define dtor ~
#define fn auto
