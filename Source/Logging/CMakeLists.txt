# --------------------------------------------------------------------------------------------------
# Atom.Logging
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE SOURCES "Private/**.cpp"
    "Private/**.h" "Public/**.h")

add_library(Atom.Logging STATIC ${SOURCES})
target_include_directories(Atom.Logging PRIVATE "Private/")
target_include_directories(Atom.Logging PUBLIC "Public/")

target_link_libraries(Atom.Logging PUBLIC Atom.Core)

# --------------------------------------------------------------------------------------------------
# Atom.Logging Tests
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE TEST_SOURCES "Tests/**.cpp")

add_executable(Atom.Logging.Tests ${TEST_SOURCES})
target_include_directories(Atom.Logging.Tests PUBLIC "Tests/")

target_link_libraries(Atom.Logging.Tests PUBLIC Atom.Logging)
target_link_libraries(Atom.Logging.Tests PUBLIC Catch2::Catch2WithMain)

add_test(Atom.Logging.Tests Atom.Logging.Tests)