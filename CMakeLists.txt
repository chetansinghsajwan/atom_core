cmake_minimum_required(VERSION 3.5)
project(atom.core VERSION 0.1.0)
enable_language(CXX)

cmake_policy(SET "CMP0076" NEW)

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
    cpptrace
    GIT_REPOSITORY "https://github.com/jeremy-rifkin/cpptrace.git"
    GIT_TAG        "v0.3.1"
    GIT_SHALLOW    true
)

FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY "https://github.com/Neargye/magic_enum.git"
    GIT_TAG        "v0.9.5"
    GIT_SHALLOW    true
)

FetchContent_Declare(
  libassert
  GIT_REPOSITORY "https://github.com/jeremy-rifkin/libassert.git"
  GIT_TAG        "v2.0.0-alpha"
)

FetchContent_MakeAvailable(catch2 fmt cpptrace magic_enum libassert)

# --------------------------------------------------------------------------------------------------
# C++20 Global Config
# --------------------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD "20")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")
enable_testing()

# --------------------------------------------------------------------------------------------------
# atom.core
# --------------------------------------------------------------------------------------------------

add_library(atom.core STATIC)
target_sources(atom.core PUBLIC "source/atom.core.cpp")
target_include_directories(atom.core PUBLIC "include/")
target_link_libraries(atom.core PUBLIC cpptrace::cpptrace)
target_link_libraries(atom.core PUBLIC fmt::fmt-header-only)
target_link_libraries(atom.core PUBLIC magic_enum::magic_enum)
target_link_libraries(atom.core PUBLIC libassert::assert)

# --------------------------------------------------------------------------------------------------
# atom.core.tests
# --------------------------------------------------------------------------------------------------

add_executable(atom.core.tests)

file(GLOB_RECURSE test_sources "tests/**.cpp")
target_sources(atom.core.tests PRIVATE "${test_sources}")
target_include_directories(atom.core.tests PRIVATE "tests/")
target_link_libraries(atom.core.tests PRIVATE atom.core)
target_link_libraries(atom.core.tests PRIVATE Catch2::Catch2WithMain)

add_test(atom.core.tests atom.core.tests)

# --------------------------------------------------------------------------------------------------
# atom.core.sandbox
# --------------------------------------------------------------------------------------------------

add_executable(atom.core.sandbox "sandbox/sandbox.cpp")
target_link_libraries(atom.core.sandbox PRIVATE atom.core)
