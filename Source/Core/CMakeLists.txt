# --------------------------------------------------------------------------------------------------
# Atom.Core
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE SOURCES "Private/**.cpp"
    "Private/**.h" "Public/**.h")

add_library(Atom.Core STATIC ${SOURCES})
target_include_directories(Atom.Core PRIVATE "Private/")
target_include_directories(Atom.Core PUBLIC "Public/")
target_compile_definitions(Atom.Core PRIVATE ATOM_EXPORT)
target_compile_definitions(Atom.Core PUBLIC _GLIBCXX_HAVE_STACKTRACE)
set_target_properties(Atom.Core PROPERTIES CXX_STANDARD "23")

target_link_libraries(Atom.Core PUBLIC WjCryptLib)
target_link_libraries(Atom.Core PUBLIC fmt)
target_link_libraries(Atom.Core PUBLIC utf-cpp)
target_link_libraries(Atom.Core PUBLIC assert)

# Library for stacktrace generation for {assert} ("dl" on Linux/OSx, "dbghelp" on Windows)
if (WIN32)
    target_link_libraries(Atom.Core PRIVATE dbghelp)
elseif(UNIX)
    target_link_libraries(Atom.Core PUBLIC dl)
endif()

# --------------------------------------------------------------------------------------------------
# Atom.Core.Tests
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE TEST_SOURCES "Tests/**.cpp")

add_executable(Atom.Core.Tests ${TEST_SOURCES})
target_include_directories(Atom.Core.Tests PUBLIC "Tests/")
set_target_properties(Atom.Core.Tests PROPERTIES CXX_STANDARD "23")

target_link_libraries(Atom.Core.Tests PUBLIC Atom.Core)
target_link_libraries(Atom.Core.Tests PUBLIC Catch2::Catch2WithMain)

add_test(Atom.Core.Tests Atom.Core.Tests)

# --------------------------------------------------------------------------------------------------
# Atom.Core.Sandbox
# --------------------------------------------------------------------------------------------------

add_executable(Atom.Core.Sandbox Sandbox.cpp)
set_target_properties(Atom.Core.Sandbox PROPERTIES CXX_STANDARD "23")
target_link_libraries(Atom.Core.Sandbox PUBLIC Atom.Core)