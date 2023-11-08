#pragma once
#include "Atom/Preprocessors.h"

namespace Atom
{
    class BuildConfig
    {
    public:
        enum class Mode
        {
            Debug,
            Release
        };

        enum class Platform
        {
            Windows,
            Posix,
            Unknown
        };

        enum class Compiler
        {
            Clang,
            ClangGnuc,
            ClangMsvc,
            Gnuc,
            Msvc,
            Unknown
        };

        class Version
        {
        public:
            unsigned int major;
            unsigned int minor;
            unsigned int patch;
        };

    public:
        static consteval auto GetMode() -> Mode
        {
            return _GetMode();
        }

        static consteval auto IsModeDebug() -> bool
        {
            return GetMode() == Mode::Debug;
        }

        static consteval auto IsModeRelease() -> bool
        {
            return GetMode() == Mode::Debug;
        }

        static consteval auto GetPlatform() -> Platform
        {
            return _GetPlatform();
        }

        static consteval auto IsPlatformWindows() -> bool
        {
            return GetPlatform() == Platform::Windows;
        }

        static consteval auto IsPlatformPosix() -> bool
        {
            return GetPlatform() == Platform::Posix;
        }

        static consteval auto GetCompiler() -> Compiler
        {
            return _GetCompiler();
        }

        static consteval auto GetCompilerVersion() -> Version
        {
            return _GetCompilerVersion();
        }

        static consteval auto IsCompilerClang() -> bool
        {
            return GetCompiler() == Compiler::Clang or GetCompiler() == Compiler::ClangMsvc
                   or GetCompiler() == Compiler::ClangGnuc;
        }

        static consteval auto IsCompilerClangGnuc() -> bool
        {
            return GetCompiler() == Compiler::ClangGnuc;
        }

        static consteval auto IsCompilerClangMsvc() -> bool
        {
            return GetCompiler() == Compiler::ClangMsvc;
        }

        static consteval auto IsCompilerGnuc() -> bool
        {
            return GetCompiler() == Compiler::Gnuc;
        }

        static consteval auto IsCompilerGnucEmulated() -> bool
        {
            return GetCompiler() == Compiler::ClangGnuc;
        }

        static consteval auto IsCompilerGnucOrEmulated() -> bool
        {
            return GetCompiler() == Compiler::Gnuc or GetCompiler() == Compiler::ClangGnuc;
        }

        static consteval auto GetCompilerGnucEmulatedVersion() -> Version
        {
            // TODO: add this assertion.
            // assert(IsCompilerGnucEmulated(), "This compiler doesn't support gcc features.");

            return _GetCompilerGnucVersion();
        }

        static consteval auto IsCompilerMsvc() -> bool
        {
            return GetCompiler() == Compiler::Msvc;
        }

        static consteval auto IsCompilerMsvcEmulated() -> bool
        {
            return GetCompiler() == Compiler::ClangMsvc;
        }

        static consteval auto IsCompilerMsvcOrEmulated() -> bool
        {
            return GetCompiler() == Compiler::Msvc || GetCompiler() == Compiler::ClangMsvc;
        }

        static consteval auto GetCompilerMsvcEmulatedVersion() -> Version
        {
            // TODO: add this assertion.
            // assert(IsCompilerMsvcEmulated(), "This compiler doesn't support msvc features.");

            return _GetCompilerMsvcVersion();
        }

    private:
        static consteval auto _GetMode() -> Mode
        {
#if defined(ATOM_MODE_DEBUG)
            return Mode::Debug;
#elif defined(ATOM_MODE_RELEASE)
            return Mode::Release;
#endif
        }

        static consteval auto _GetPlatform() -> Platform
        {
#if defined(ATOM_PLATFORM_WIN)
            return Platform::Windows;
#elif defined(ATOM_PLATFORM_POSIX)
            return Platform::Posix;
#elif defined(ATOM_PLATFORM_UNKNOWN)
            return Platform::Unknown;
#endif
        }

        static consteval auto _GetCompiler() -> Compiler
        {
#if defined(ATOM_COMPILER_CLANG)
#    if defined(ATOM_COMPILER_MSVC_EMULATED)
            return Compiler::ClangMsvc;
#    elif defined(ATOM_COMPILER_GNUC_EMULATED)
            return Compiler::ClangGnuc;
#    else
            return Compiler::Clang;
#    endif
#elif defined(ATOM_COMPILER_GNUC)
            return Compiler::Gnuc;
#elif defined(ATOM_COMPILER_MSVC)
            return Compiler::Msvc;
#endif
        }

        static consteval auto _GetCompilerClangVersion() -> Version
        {
#if defined(ATOM_COMPILER_CLANG)
            return Version{ .major = ATOM_COMPILER_CLANG_VER_MAJOR,
                .minor = ATOM_COMPILER_CLANG_VER_MINOR,
                .patch = ATOM_COMPILER_CLANG_VER_PATCH };
#else
            return _GetCompilerUnknownVersion();
#endif
        }

        static consteval auto _GetCompilerGnucVersion() -> Version
        {
#if defined(ATOM_COMPILER_GNUC) or defined(ATOM_COMPILER_GNUC_EMULATED)
            return Version{ .major = ATOM_COMPILER_GNUC_VER_MAJOR,
                .minor = ATOM_COMPILER_GNUC_VER_MINOR,
                .patch = ATOM_COMPILER_GNUC_VER_PATCH };
#else
            return _GetCompilerUnknownVersion();
#endif
        }

        static consteval auto _GetCompilerMsvcVersion() -> Version
        {
#if defined(ATOM_COMPILER_MSVC) or defined(ATOM_COMPILER_MSVC_EMULATED)
            return Version{ .major = ATOM_COMPILER_MSVC_VER_MAJOR,
                .minor = ATOM_COMPILER_MSVC_VER_MINOR,
                .patch = ATOM_COMPILER_MSVC_VER_PATCH };
#else
            return _GetCompilerUnknownVersion();
#endif
        }

        static consteval auto _GetCompilerUnknownVersion() -> Version
        {
            return Version{};
        }

        static consteval auto _GetCompilerVersion() -> Version
        {
#if defined(ATOM_COMPILER_CLANG)
            return _GetCompilerClangVersion();
#elif defined(ATOM_COMPILER_GNUC)
            return _GetCompilerGnucVersion();
#elif defined(ATOM_COMPILER_MSVC)
            return _GetCompilerMsvcVersion();
#elif defined(ATOM_COMPILER_UNKNOWN)
            return _GetCompilerUnknownVersion();
#endif
        }
    };
}
