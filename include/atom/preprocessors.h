// #pragma once

/// ------------------------------------------------------------------------------------------------
///
/// ------------------------------------------------------------------------------------------------
#define ATOM_ALIAS(THIS_TYPE, ...)                                                                 \
    class THIS_TYPE: public __VA_ARGS__                                                            \
    {                                                                                              \
        using base_type = __VA_ARGS__;                                                             \
                                                                                                   \
    public:                                                                                        \
        using base_type::base_type;                                                                \
        using base_type::operator=;                                                                \
    };

/// ------------------------------------------------------------------------------------------------
/// General utility macro
/// ------------------------------------------------------------------------------------------------
#define ATOM_CAT(A, B) A##B
#define ATOM_EXPAND(...) __VA_ARGS__

/// ------------------------------------------------------------------------------------------------
///
/// ------------------------------------------------------------------------------------------------
#define _ATOM_RSEQ_N()                                                                             \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41,    \
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19,    \
        18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define _ATOM_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17,    \
    _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, \
    _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, \
    _56, _57, _58, _59, _60, _61, _62, _63, N, ...)                                                \
    N

#define _PP_ARG_GET_N(...) _ATOM_ARG_N(__VA_ARGS__)
#define ATOM_ARG_N(...) _PP_ARG_GET_N(__VA_ARGS__, _ATOM_RSEQ_N())

/// ------------------------------------------------------------------------------------------------
/// Macro overloading feature support.
///
/// # Example
///
/// ```cpp
///
///     #define FOO(...)       ATOM_OVERLOAD(FOO, __VA_ARGS__)
///     #define FOO_0()        "Zero"
///     #define FOO_1(X)       "One"
///     #define FOO_2(X, Y)    "Two"
///     #define FOO_3(X, Y, Z) "Three"
///
/// ```
/// ------------------------------------------------------------------------------------------------
#define _ATOM_OVERLOAD_SELECT(NAME, NUM) ATOM_CAT(NAME##_, NUM)
#define ATOM_OVERLOAD(NAME, ...) _ATOM_OVERLOAD_SELECT(NAME, ATOM_ARG_N(__VA_ARGS__))(__VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Mode
/// ------------------------------------------------------------------------------------------------
#if defined(NDEBUG)
#    define ATOM_MODE_DEBUG
#else
#    define ATOM_MODE_RELEASE
#endif

/// ------------------------------------------------------------------------------------------------
/// Platform
/// ------------------------------------------------------------------------------------------------
#if defined(_WIN32)
#    define ATOM_PLATFORM_WIN
#elif defined(__unix__)
#    define ATOM_PLATFORM_POSIX
#else
#    define ATOM_PLATFORM_UNKNOWN
#endif

/// ------------------------------------------------------------------------------------------------
/// Compiler
/// ------------------------------------------------------------------------------------------------
#if defined(__clang__)
#    define ATOM_COMPILER_CLANG
#    define ATOM_COMPILER_CLANG_VER_MAJOR __clang_major__
#    define ATOM_COMPILER_CLANG_VER_MINOR __clang_minor__
#    define ATOM_COMPILER_CLANG_VER_PATCH __clang_patchlevel__
#    define ATOM_COMPILER_CLANG_VER __clang_version__

#elif defined(__GNUC__)
#    if defined(ATOM_COMPILER_CLANG)
#        define ATOM_COMPILER_GNUC_EMUL
#    else
#        define ATOM_COMPILER_GNUC
#    endif
#    define ATOM_COMPILER_GNUC_VER_MAJOR __GNUC__
#    define ATOM_COMPILER_GNUC_VER_MINOR __GNUC_MINOR__
#    define ATOM_COMPILER_GNUC_VER_PATCH __GNUC_PATCHLEVEL__
#    define ATOM_COMPILER_GNUC_VER (__GNUC__ * 100) + __GNUC_MINOR__

#elif defined(_MSC_VER)
#    if defined(ATOM_COMPILER_CLANG)
#        define ATOM_COMPILER_MSVC_EMUL
#    else
#        define ATOM_COMPILER_MSVC
#    endif
#    define ATOM_COMPILER_MSVC_VER_MAJOR (_MSC_VER / 100)
#    define ATOM_COMPILER_MSVC_VER_MINOR (_MSC_VER % 100)
#    define ATOM_COMPILER_MSVC_VER_PATCH (_MSC_VER)
#    define ATOM_COMPILER_MSVC_VER _MSC_VER

#else
#    define ATOM_COMPILER_UNKNOWN

#endif

/// ------------------------------------------------------------------------------------------------
/// Optimization handling macros.
///
/// # Macros
/// - `ATOM_PRAGMA_OPTIMIZE_OFF`: Disables all optimization until [`ATOM_PRAGMA_OPTIMIZE_ON`] is
///   encountered.
/// - `ATOM_PRAGMA_OPTIMIZE_ON`: Enables optimizations disabled by [`ATOM_PRAGMA_OPTIMIZE_OFF`].
/// ------------------------------------------------------------------------------------------------
#if defined(ATOM_COMPILER_CLANG)
#    define ATOM_PRAGMA_OPTIMIZE_OFF _Pragma("clang optimize off")
#    define ATOM_PRAGMA_OPTIMIZE_ON _Pragma("clang optimize on")
#elif defined(ATOM_COMPILER_GNUC)
#    define ATOM_PRAGMA_OPTIMIZE_OFF _Pragma("GCC push_options") _Pragma("GCC optimize(\"O0\")")
#    define ATOM_PRAGMA_OPTIMIZE_ON _Pragma("GCC pop_options")
#elif defined(ATOM_COMPILER_MSVC)
#    define ATOM_PRAGMA_OPTIMIZE_OFF _Pragma("optimize(\"\", off)");
#    define ATOM_PRAGMA_OPTIMIZE_ON _Pragma("optimize(\"\", on)");
#endif

/// ------------------------------------------------------------------------------------------------
/// ATOM_ATTR_NO_UNIQUE_ADDRESS
/// ------------------------------------------------------------------------------------------------
#if defined(ATOM_COMPILER_CLANG)
#    define ATOM_ATTR_NO_UNIQUE_ADDRESS
#elif defined(ATOM_COMPILER_MSVC)
#    if (ATOM_COMPILER_MSVC_VER >= 1929)
#        define ATOM_ATTR_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#    else
#        define ATOM_ATTR_NO_UNIQUE_ADDRESS [[no_unique_address]]
#    endif
#else
#    define ATOM_ATTR_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

/// ------------------------------------------------------------------------------------------------
/// ATOM_CONDITIONAL_FIELD
/// ------------------------------------------------------------------------------------------------
#define ATOM_CONDITIONAL_FIELD(...)                                                                \
    ATOM_ATTR_NO_UNIQUE_ADDRESS                                                                    \
    std::conditional_t<__VA_ARGS__, bool>
//     _atom::conditional_field<(Condition), type>

// namespace _atom
// {
//     class _empty
//     {};

//     template <bool cond, typename type>
//     using conditional_field = std::conditional_t<cond, type, _empty>;
// }
