# --------------------------------------------------------------------------------------------------
# Atom.Logging
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE SOURCES "Private/**.cpp"
    "Private/**.h" "Public/**.h")

add_library(Atom.Logging SHARED ${SOURCES})
target_include_directories(Atom.Logging PRIVATE "Private/")
target_include_directories(Atom.Logging PUBLIC "Public/")
set_target_properties(Atom.Logging PROPERTIES CXX_STANDARD "20")

target_link_libraries(Atom.Logging PUBLIC Atom.Core)
target_link_libraries(Atom.Logging PUBLIC fmt)