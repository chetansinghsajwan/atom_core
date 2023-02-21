#pragma once

//// -----------------------------------------------------------------------------------------------
//// Documentation Order
//// -----------------------------------------------------------------------------------------------

/// ------------------------------------------------------------------------------------------------
/// @BRIEF
/// 
/// @TPARAM
/// @PARAM[IN]
/// @PARAM[OUT]
/// 
/// @THROWS
/// 
/// @EXCEPTION SAFETY [NOEXCEPT, STRONG, WEAK, NONE]
/// 
/// @THREAD_SAFETY [SAFE, CONDITIONAL, NONE]
/// 
/// @TIME_COMPLEXITY
/// 
/// @EXAMPLES
/// 
/// @TODO
/// ------------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
//// MACROS
//// -----------------------------------------------------------------------------------------------

#define ATOM_CONFIG_DEBUG
#define FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#define FORWARD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define ATOM_FUNCTION __func__

#define STATIC_ASSERT_MSG(assertion, msg) static_assert(assertion, msg)
#define STATIC_ASSERT(assertion) static_assert(assertion)
#define STATIC_THROW(msg) static_assert(false, msg)

namespace Atom
{
    using byte = unsigned char;
    using SizeT = std::size_t;
    using DiffT = long long;

    template <typename T>
    T&& MOVE(T& obj) noexcept
    {
        return std::move(obj);
    }
}