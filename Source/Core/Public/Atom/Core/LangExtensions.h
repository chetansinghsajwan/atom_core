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
#define ATOM_IF_DEBUG if constexpr (ATOM_IS_CONFIG_DEBUG)

#define ATOM_FUNCTION __func__

#define ofinal override final
#define fwd(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
#define mov(...) static_cast<Atom::_remove_ref_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#define declval(T) std::declval<T>()
#define ctor
#define dtor ~
#define fn auto
#define abstract = 0
#define extends : public
#define self (*this)