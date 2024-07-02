cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(atom_core VERSION 0.1.0)
enable_language(CXX)

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0155 NEW)

# --------------------------------------------------------------------------------------------------
# options
# --------------------------------------------------------------------------------------------------

option(atom_core_build_tests "Enable this to build tests." OFF)

# --------------------------------------------------------------------------------------------------
# atom_core
# --------------------------------------------------------------------------------------------------

find_package(fmt REQUIRED)
find_package(cpptrace REQUIRED)
find_package(magic_enum REQUIRED)

add_library(atom_core STATIC)

file(GLOB_RECURSE atom_core_modules "sources/**.cppm")
target_sources(atom_core PUBLIC FILE_SET CXX_MODULES FILES ${atom_core_modules})

target_include_directories(atom_core PRIVATE "include")
target_link_libraries(atom_core PRIVATE
    "fmt::fmt-header-only"
    "magic_enum::magic_enum"
    "cpptrace::cpptrace")

target_compile_features(atom_core PUBLIC "cxx_std_23")
target_compile_options(
    atom_core
    PUBLIC # some warning in libcxx
           "-Wno-deprecated-declarations"
           # to declare std module
           "-Wno-reserved-module-identifier"
           "-ferror-limit=1000")

# --------------------------------------------------------------------------------------------------
# atom_core_sandbox
# --------------------------------------------------------------------------------------------------

add_executable(atom_core_sandbox)
target_sources(atom_core_sandbox PRIVATE "sandbox/sandbox.cpp")
target_link_libraries(atom_core_sandbox PRIVATE atom_core)

# --------------------------------------------------------------------------------------------------
# testing
# --------------------------------------------------------------------------------------------------

if(atom_core_build_tests)
    find_package(Catch2 REQUIRED)

    add_executable(atom_core_tests)

    file(GLOB_RECURSE atom_core_tests_modules "tests/**.cppm" "tests/**.cxx")
    target_sources(atom_core_tests PRIVATE FILE_SET CXX_MODULES FILES "${atom_core_tests_modules}")

    target_include_directories(atom_core_tests PRIVATE "tests/")
    target_link_libraries(atom_core_tests PRIVATE atom_core Catch2::Catch2WithMain)

    add_test(atom_core_tests atom_core_tests)
endif()

# --------------------------------------------------------------------------------------------------
# install phase
# --------------------------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "atom_core-config-version.cmake"
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(
    TARGETS atom_core
    EXPORT atom_core_targets
    FILE_SET CXX_MODULES
    DESTINATION "lib"
    INCLUDES
    DESTINATION "include")

install(
    EXPORT atom_core_targets
    FILE "atom_core-targets.cmake"
    DESTINATION "cmake/")

install(DIRECTORY "include/" DESTINATION "include/")

install(FILES "cmake/atom_core-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/atom_core-config-version.cmake" DESTINATION "cmake/")
