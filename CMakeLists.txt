cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(atom.core VERSION 0.1.0)
enable_language(CXX)

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0155 NEW)

find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)
find_package(magic_enum REQUIRED)

# --------------------------------------------------------------------------------------------------
# atom.core
# --------------------------------------------------------------------------------------------------

add_library(atom.core STATIC)

file(GLOB_RECURSE modules "source/**.cppm" "include/**.cppm")
target_sources(atom.core PUBLIC FILE_SET CXX_MODULES FILES ${modules})

target_include_directories(atom.core PRIVATE "include/")
target_link_libraries(atom.core PRIVATE "fmt::fmt-header-only" "magic_enum::magic_enum")
target_compile_features(atom.core PUBLIC "cxx_std_23")
target_compile_options(atom.core PUBLIC "-Wno-deprecated-declarations"
                                        "-Wno-reserved-module-identifier")

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

# --------------------------------------------------------------------------------------------------
# install phase
# --------------------------------------------------------------------------------------------------

install(
    TARGETS atom.core
    EXPORT atom_core-targets
    FILE_SET CXX_MODULES
    DESTINATION "lib")

install(
    EXPORT atom_core-targets
    FILE "atom_core-config.cmake"
    NAMESPACE "atom::"
    DESTINATION "cmake/")
