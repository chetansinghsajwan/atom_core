cmake_minimum_required(VERSION 3.5)
project(Atom.Core VERSION 0.1.0 LANGUAGES CXX)

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

FetchContent_MakeAvailable(catch2 fmt wjcrypt)

# --------------------------------------------------------------------------------------------------
# C++20 Global Config
# --------------------------------------------------------------------------------------------------
include("cmake/enable_modules.cmake")
set(CMAKE_CXX_STANDARD "23")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")
enable_testing()

# --------------------------------------------------------------------------------------------------
# Atom.Core
# --------------------------------------------------------------------------------------------------

add_library(Atom.Core STATIC)
target_sources(Atom.Core
    PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
    "src/core.cpp"
)
target_include_directories(Atom.Core PRIVATE "src/")

target_link_libraries(Atom.Core PRIVATE WjCryptLib)
target_link_libraries(Atom.Core PRIVATE fmt::fmt-header-only)

# --------------------------------------------------------------------------------------------------
# Atom.Core.Tests
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE TEST_SOURCES "tests/**.cpp")

add_executable(Atom.Core.Tests ${TEST_SOURCES})
target_include_directories(Atom.Core.Tests PUBLIC "tests/")

target_link_libraries(Atom.Core.Tests PUBLIC Atom.Core)
target_link_libraries(Atom.Core.Tests PUBLIC Catch2::Catch2WithMain)

add_test(Atom.Core.Tests Atom.Core.Tests)

# --------------------------------------------------------------------------------------------------
# Atom.Core.Sandbox
# --------------------------------------------------------------------------------------------------

add_executable(Atom.Core.Sandbox "sandbox/sandbox.cpp")
target_include_directories(Atom.Core.Sandbox PRIVATE "sandbox/")
target_link_libraries(Atom.Core.Sandbox PUBLIC Atom.Core)
