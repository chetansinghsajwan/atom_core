#pragma once

namespace Atom
{
    template <typename T>
    struct _remove_ref { using value = T; };

    template <typename T>
    struct _remove_ref<T&> { using value = T; };

    template <typename T>
    struct _remove_ref<T&&> { using value = T; };

    template <typename T>
    using _remove_ref_t = _remove_ref<T>::value;
}

#define ATOM_IS_CONFIG_DEBUG true
#define ATOM_IF_DEBUG if cexpr (ATOM_IS_CONFIG_DEBUG)

#define ATOM_FUNCTION __func__

#define tname typename
#define noex noexcept
#define noex_if(...) noex(noex(__VA_ARGS__))
#define is_noex(...) noex(__VA_ARGS__)
#define ofinal override final
#define fwd(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
#define mov(...) static_cast<Atom::_remove_ref_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#define declval(T) std::declval<T>()
#define noret [[noreturn]]
#define ctor
#define dtor ~
#define fn auto
#define abstract = 0
#define cexpr constexpr
#define ceval consteval
#define cinit constinit
#define extends : public
#define pub public:
#define prot protected:
#define priv private:
#define op operator

// #define sassert(...) static_assert(__VA_ARGS__)
// #define scast(...) static_cast(__VA_ARGS__)
// #define dcast(...) static_cast(__VA_ARGS__)
// #define rcast(...) static_cast(__VA_ARGS__)
// #define ccast(...) static_cast(__VA_ARGS__)

// #define use using
// #define var auto
// #define let const auto
// #define inl inline
// #define finl inline
// #define and &&
// #define or ||
// #define not !
// #define null nullptr
// #define init :
// #define ret return
// #define this (*this)
// #define base ((Base&)this)
// #define enum enum class