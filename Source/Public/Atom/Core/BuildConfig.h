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
            ClangGcc,
            ClangMsvc,
            Gcc,
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
                   or GetCompiler() == Compiler::ClangGcc;
        }

        static consteval auto IsCompilerClangGcc() -> bool
        {
            return GetCompiler() == Compiler::ClangGcc;
        }

        static consteval auto IsCompilerClangMsvc() -> bool
        {
            return GetCompiler() == Compiler::ClangMsvc;
        }

        static consteval auto IsCompilerGcc() -> bool
        {
            return GetCompiler() == Compiler::Gcc;
        }

        static consteval auto IsCompilerGccEmulated() -> bool
        {
            return GetCompiler() == Compiler::ClangGcc;
        }

        static consteval auto IsCompilerGccOrEmulated() -> bool
        {
            return GetCompiler() == Compiler::Gcc or GetCompiler() == Compiler::ClangGcc;
        }

        static consteval auto GetCompilerGccEmulatedVersion() -> Version
        {
            // TODO: add this assertion.
            // assert(IsCompilerGccEmulated(), "This compiler doesn't support gcc features.");

            return _GetCompilerGccVersion();
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
#if defined(NDEBUG)
            return Mode::Release;
#else
            return Mode::Debug;
#endif
        }

        static consteval auto _GetPlatform() -> Platform
        {
#if defined(_WIN32)
            return Platform::Windows;
#elif defined(__unix__)
            return Platform::Posix;
#else
            return Platform::Unknown;
#endif
        }

        static consteval auto _GetCompiler() -> Compiler
        {
#if defined(__clang__)
#    if defined(_MSC_VER)
            return Compiler::ClangMsvc;
#    elif defined(__GNUC__)
            return Compiler::ClangGcc;
#    else
            return Compiler::Clang;
#    endif
#elif defined(__GNUC__)
            return Compiler::Gcc;
#elif defined(_MSC_VER)
            return Compiler::Msvc;
#endif
        }

        static consteval auto _GetCompilerClangVersion() -> Version
        {
#if defined(__clang__)
            return Version{
                .major = __clang_major__, .minor = __clang_minor__, .patch = __clang_patchlevel__
            };
#else
            return _GetCompilerUnknownVersion();
#endif
        }

        static consteval auto _GetCompilerGccVersion() -> Version
        {
#if defined(__GNUC__)
            return Version{
                .major = __GNUC__, .minor = __GNUC_MINOR__, .patch = __GNUC_PATCHLEVEL__
            };
#else
            return _GetCompilerUnknownVersion();
#endif
        }

        static consteval auto _GetCompilerMsvcVersion() -> Version
        {
#if defined(_MSC_VER)
            return Version{ .major = _MSC_VER / 100, .minor = _MSC_VER % 100, .patch = 0 };
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
#if defined(__clang__)
            return _GetCompilerClangVersion();
#elif defined(__GNUC__)
            return _GetCompilerGccVersion();
#elif defined(_MSC_VER)
            return _GetCompilerMsvcVersion();
#endif
        }
    };
}
