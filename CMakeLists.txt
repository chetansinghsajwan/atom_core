cmake_minimum_required(VERSION 3.5)
project(atom.core VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG        "v3.5.1"
    GIT_SHALLOW    true
)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
    GIT_TAG        "10.2.1"
    GIT_SHALLOW    true
)

FetchContent_Declare(
    wjcrypt
    GIT_REPOSITORY "https://github.com/WaterJuice/WjCryptLib.git"
    GIT_TAG        "Version_2.3.0"
    GIT_SHALLOW    true
)

FetchContent_Declare(
    cpptrace
    GIT_REPOSITORY "https://github.com/jeremy-rifkin/cpptrace.git"
    GIT_TAG        "v0.3.1"
    GIT_SHALLOW    true
)

FetchContent_MakeAvailable(catch2 fmt wjcrypt cpptrace)

# --------------------------------------------------------------------------------------------------
# C++20 Global Config
# --------------------------------------------------------------------------------------------------
include("cmake/enable_modules.cmake")
set(CMAKE_CXX_STANDARD "23")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")
enable_testing()

# --------------------------------------------------------------------------------------------------
# atom.core
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE sources "src/**.cpp")

add_library(atom.core STATIC)
target_sources(atom.core
    PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
    "${sources}"
)
target_include_directories(atom.core PRIVATE "src/")

target_link_libraries(atom.core PRIVATE cpptrace::cpptrace)
target_link_libraries(atom.core PRIVATE WjCryptLib)
target_link_libraries(atom.core PRIVATE fmt::fmt-header-only)
target_compile_options(atom.core PUBLIC "-Wno-read-modules-implicitly")
target_compile_options(atom.core PUBLIC "-ferror-limit=1000")

# --------------------------------------------------------------------------------------------------
# atom.core.tests
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE test_sources "tests/**.cpp")

add_executable(atom.core.tests ${test_sources})
target_include_directories(atom.core.tests PRIVATE "tests/")

target_link_libraries(atom.core.tests PRIVATE atom.core)
target_link_libraries(atom.core.tests PRIVATE Catch2::Catch2WithMain)

add_test(atom.core.tests atom.core.tests)

# --------------------------------------------------------------------------------------------------
# atom.core.sandbox
# --------------------------------------------------------------------------------------------------

add_executable(atom.core.sandbox "sandbox/sandbox.cpp")
target_include_directories(atom.core.sandbox PRIVATE "sandbox/")
target_link_libraries(atom.core.sandbox PRIVATE atom.core)
