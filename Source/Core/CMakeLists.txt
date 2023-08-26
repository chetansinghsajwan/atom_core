# --------------------------------------------------------------------------------------------------
# Atom.Core
# --------------------------------------------------------------------------------------------------

find_package(fmt CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "Private/**.cpp"
    "Private/**.h" "Public/**.h")

add_library(Atom.Core STATIC ${SOURCES})
target_include_directories(Atom.Core PRIVATE "Private/")
target_include_directories(Atom.Core PUBLIC "Public/")
target_compile_definitions(Atom.Core PRIVATE ATOM_EXPORT)

target_link_libraries(Atom.Core PUBLIC WjCryptLib)
target_link_libraries(Atom.Core PUBLIC fmt::fmt-header-only)
target_link_libraries(Atom.Core PUBLIC utf-cpp)

# --------------------------------------------------------------------------------------------------
# Atom.Core.Tests
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE TEST_SOURCES "Tests/**.cpp")

add_executable(Atom.Core.Tests ${TEST_SOURCES})
target_include_directories(Atom.Core.Tests PUBLIC "Tests/")

target_link_libraries(Atom.Core.Tests PUBLIC Atom.Core)
target_link_libraries(Atom.Core.Tests PUBLIC Catch2::Catch2WithMain)

add_test(Atom.Core.Tests Atom.Core.Tests)

# --------------------------------------------------------------------------------------------------
# Atom.Core.Sandbox
# --------------------------------------------------------------------------------------------------

add_executable(Atom.Core.Sandbox "Sandbox/Sandbox.cpp")
target_include_directories(Atom.Core.Sandbox PRIVATE "Sandbox")
target_link_libraries(Atom.Core.Sandbox PUBLIC Atom.Core)