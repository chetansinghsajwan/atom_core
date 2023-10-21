cmake_minimum_required(VERSION 3.5)
project(Atom.Core VERSION 0.1.0 LANGUAGES CXX C)

include(CMake/CPM.cmake)
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/Build/CPMCache")

CPMAddPackage("gh:fmtlib/fmt#10.1.0")
CPMAddPackage("gh:catchorg/Catch2@3.4.0")
CPMAddPackage("gh:WaterJuice/WjCryptLib#Version_2.3.0")

# --------------------------------------------------------------------------------------------------
# C++20 Global Config
# --------------------------------------------------------------------------------------------------
# set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
# set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP "1")
set(CMAKE_CXX_STANDARD "23")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")
enable_testing()

# --------------------------------------------------------------------------------------------------
# Atom.Core
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE SOURCES "Source/Private/**.cpp"
    "Source/Private/**.h" "Public/**.h")

add_library(Atom.Core STATIC ${SOURCES})
target_include_directories(Atom.Core PRIVATE "Source/Private/")
target_include_directories(Atom.Core PUBLIC "Source/Public/")
target_compile_definitions(Atom.Core PRIVATE ATOM_EXPORT)

target_link_libraries(Atom.Core PUBLIC WjCryptLib)
target_link_libraries(Atom.Core PUBLIC fmt::fmt-header-only)

# --------------------------------------------------------------------------------------------------
# Atom.Core.Tests
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE TEST_SOURCES "Source/Tests/**.cpp")

add_executable(Atom.Core.Tests ${TEST_SOURCES})
target_include_directories(Atom.Core.Tests PUBLIC "Source/Tests/")

target_link_libraries(Atom.Core.Tests PUBLIC Atom.Core)
target_link_libraries(Atom.Core.Tests PUBLIC Catch2::Catch2WithMain)

add_test(Atom.Core.Tests Atom.Core.Tests)

# --------------------------------------------------------------------------------------------------
# Atom.Core.Sandbox
# --------------------------------------------------------------------------------------------------

add_executable(Atom.Core.Sandbox "Source/Sandbox/Sandbox.cpp")
target_include_directories(Atom.Core.Sandbox PRIVATE "Sandbox")
target_link_libraries(Atom.Core.Sandbox PUBLIC Atom.Core)