cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(atom.core VERSION 0.1.0)
enable_language(CXX)

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0155 NEW)

include(FetchContent)
set(FETCHCONTENT_QUIET "ON")

set(CMAKE_CXX_STANDARD "23")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")
enable_testing()

FetchContent_Declare(
    catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG "v3.5.1"
    GIT_PROGRESS true
    GIT_SHALLOW true)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
    GIT_TAG "10.2.1"
    GIT_PROGRESS true
    GIT_SHALLOW true)

FetchContent_Declare(
    cpptrace
    GIT_REPOSITORY "https://github.com/jeremy-rifkin/cpptrace.git"
    GIT_TAG "v0.3.1"
    GIT_PROGRESS true
    GIT_SHALLOW true)

FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY "https://github.com/Neargye/magic_enum.git"
    GIT_TAG "v0.9.5"
    GIT_PROGRESS true
    GIT_SHALLOW true)

FetchContent_Declare(
    libassert
    GIT_REPOSITORY "https://github.com/jeremy-rifkin/libassert.git"
    GIT_TAG "v2.0.0-alpha"
    GIT_PROGRESS true
    GIT_SHALLOW true)

FetchContent_MakeAvailable(catch2 fmt cpptrace magic_enum libassert)

# --------------------------------------------------------------------------------------------------
# atom.core
# --------------------------------------------------------------------------------------------------

add_library(atom.core STATIC)
target_sources(
    atom.core
    PUBLIC FILE_SET
           CXX_MODULES
           FILES
           "source/atom.core.cppm"
           "source/std.cppm"
           "source/fmt.cppm"
           "source/libassert.cppm"
           "source/magic_enum.cppm"
           "source/cpptrace.cppm")

target_include_directories(atom.core PRIVATE "include/")
target_link_libraries(atom.core PRIVATE cpptrace::cpptrace fmt::fmt-header-only
                                        magic_enum::magic_enum libassert::assert)

# temporary fix for clang-scan-dep not finding std headers in nix.
target_compile_options(
    atom.core
    PRIVATE "-I/nix/store/p0siwwr4pfh6d9026ygvagvp43gyck69-libcxx-18.1.3-dev/include/c++/v1"
           "-I/nix/store/ij144ma6vs8acil8r9hgr8xkb1dp9azg-glibc-2.39-5-dev/include")

# --------------------------------------------------------------------------------------------------
# atom.core.tests
# --------------------------------------------------------------------------------------------------

add_executable(atom.core.tests)

file(GLOB_RECURSE test_sources "tests/**.cpp")
target_sources(atom.core.tests PRIVATE "${test_sources}")
target_include_directories(atom.core.tests PRIVATE "tests/")
target_link_libraries(atom.core.tests PRIVATE atom.core Catch2::Catch2WithMain)

add_test(atom.core.tests atom.core.tests)

# --------------------------------------------------------------------------------------------------
# atom.core.sandbox
# --------------------------------------------------------------------------------------------------

add_executable(atom.core.sandbox)
target_sources(atom.core.sandbox PRIVATE "sandbox/sandbox.cpp")
target_link_libraries(atom.core.sandbox PRIVATE atom.core)
