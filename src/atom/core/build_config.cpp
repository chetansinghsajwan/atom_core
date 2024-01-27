module;
#include "atom/preprocessors.h"

export module atom.core:build_config;

namespace atom
{
    export class build_config
    {
    public:
        enum class mode
        {
            debug,
            release
        };

        enum class platform
        {
            windows,
            posix,
            unknown
        };

        enum class compiler
        {
            clang,
            clang_gnuc,
            clang_msvc,
            gnuc,
            msvc,
            unknown
        };

        class version
        {
        public:
            unsigned int major;
            unsigned int minor;
            unsigned int patch;
        };

    public:
        static consteval auto get_mode() -> mode
        {
            return _get_mode();
        }

        static consteval auto is_mode_debug() -> bool
        {
            return get_mode() == mode::debug;
        }

        static consteval auto is_mode_release() -> bool
        {
            return get_mode() == mode::debug;
        }

        static consteval auto get_platform() -> platform
        {
            return _get_platform();
        }

        static consteval auto is_platform_windows() -> bool
        {
            return get_platform() == platform::windows;
        }

        static consteval auto is_platform_posix() -> bool
        {
            return get_platform() == platform::posix;
        }

        static consteval auto get_compiler() -> compiler
        {
            return _get_compiler();
        }

        static consteval auto get_compiler_version() -> version
        {
            return _get_compiler_version();
        }

        static consteval auto is_compiler_clang() -> bool
        {
            return get_compiler() == compiler::clang or get_compiler() == compiler::clang_msvc
                   or get_compiler() == compiler::clang_gnuc;
        }

        static consteval auto is_compiler_clang_gnuc() -> bool
        {
            return get_compiler() == compiler::clang_gnuc;
        }

        static consteval auto is_compiler_clang_msvc() -> bool
        {
            return get_compiler() == compiler::clang_msvc;
        }

        static consteval auto is_compiler_gnuc() -> bool
        {
            return get_compiler() == compiler::gnuc;
        }

        static consteval auto is_compiler_gnuc_emulated() -> bool
        {
            return get_compiler() == compiler::clang_gnuc;
        }

        static consteval auto is_compiler_gnuc_or_emulated() -> bool
        {
            return get_compiler() == compiler::gnuc or get_compiler() == compiler::clang_gnuc;
        }

        static consteval auto get_compiler_gnuc_emulated_version() -> version
        {
            // todo: add this assertion.
            // assert(is_compiler_gnuc_emulated(), "this compiler does not support gcc features.");

            return _get_compiler_gnuc_version();
        }

        static consteval auto is_compiler_msvc() -> bool
        {
            return get_compiler() == compiler::msvc;
        }

        static consteval auto is_compiler_msvc_emulated() -> bool
        {
            return get_compiler() == compiler::clang_msvc;
        }

        static consteval auto is_compiler_msvc_or_emulated() -> bool
        {
            return get_compiler() == compiler::msvc || get_compiler() == compiler::clang_msvc;
        }

        static consteval auto get_compiler_msvc_emulated_version() -> version
        {
            // todo: add this assertion.
            // assert(is_compiler_msvc_emulated(), "this compiler does not support msvc features.");

            return _get_compiler_msvc_version();
        }

    private:
        static consteval auto _get_mode() -> mode
        {
#if defined(ATOM_MODE_DEBUG)
            return mode::debug;
#elif defined(ATOM_MODE_RELEASE)
            return mode::release;
#endif
        }

        static consteval auto _get_platform() -> platform
        {
#if defined(ATOM_PLATFORM_WIN)
            return platform::windows;
#elif defined(ATOM_PLATFORM_POSIX)
            return platform::posix;
#elif defined(ATOM_PLATFORM_UNKNOWN)
            return platform::unknown;
#endif
        }

        static consteval auto _get_compiler() -> compiler
        {
#if defined(ATOM_COMPILER_CLANG)
#    if defined(ATOM_COMPILER_MSVC_EMULATED)
            return compiler::clang_msvc;
#    elif defined(ATOM_COMPILER_GNUC_EMULATED)
            return compiler::clang_gnuc;
#    else
            return compiler::clang;
#    endif
#elif defined(ATOM_COMPILER_GNUC)
            return compiler::gnuc;
#elif defined(ATOM_COMPILER_MSVC)
            return compiler::msvc;
#endif
        }

        static consteval auto _get_compiler_clang_version() -> version
        {
#if defined(ATOM_COMPILER_CLANG)
            return version{ .major = ATOM_COMPILER_CLANG_VER_MAJOR,
                .minor = ATOM_COMPILER_CLANG_VER_MINOR,
                .patch = ATOM_COMPILER_CLANG_VER_PATCH };
#else
            return _get_compiler_unknown_version();
#endif
        }

        static consteval auto _get_compiler_gnuc_version() -> version
        {
#if defined(ATOM_COMPILER_GNUC) or defined(ATOM_COMPILER_GNUC_EMULATED)
            return version{ .major = ATOM_COMPILER_GNUC_VER_MAJOR,
                .minor = ATOM_COMPILER_GNUC_VER_MINOR,
                .patch = ATOM_COMPILER_GNUC_VER_PATCH };
#else
            return _get_compiler_unknown_version();
#endif
        }

        static consteval auto _get_compiler_msvc_version() -> version
        {
#if defined(ATOM_COMPILER_MSVC) or defined(ATOM_COMPILER_MSVC_EMULATED)
            return version{ .major = ATOM_COMPILER_MSVC_VER_MAJOR,
                .minor = ATOM_COMPILER_MSVC_VER_MINOR,
                .patch = ATOM_COMPILER_MSVC_VER_PATCH };
#else
            return _get_compiler_unknown_version();
#endif
        }

        static consteval auto _get_compiler_unknown_version() -> version
        {
            return version();
        }

        static consteval auto _get_compiler_version() -> version
        {
#if defined(ATOM_COMPILER_CLANG)
            return _get_compiler_clang_version();
#elif defined(ATOM_COMPILER_GNUC)
            return _get_compiler_gnuc_version();
#elif defined(ATOM_COMPILER_MSVC)
            return _get_compiler_msvc_version();
#elif defined(atom_compiler_unknown)
            return _get_compiler_unknown_version();
#endif
        }
    };
}
