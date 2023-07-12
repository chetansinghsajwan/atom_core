#pragma once

#define tname typename
#define noex noexcept
#define noex_if(...) noex(noex(__VA_ARGS__))
#define is_noex(...) noex(__VA_ARGS__)
