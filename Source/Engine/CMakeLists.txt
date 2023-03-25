# --------------------------------------------------------------------------------------------------
# Atom.Engine
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE SOURCES "Private/**.cpp"
    "Private/**.h" "Public/**.h")

add_library(Atom.Engine STATIC ${SOURCES})
target_include_directories(Atom.Engine PRIVATE "Private/")
target_include_directories(Atom.Engine PUBLIC "Public/")
set_target_properties(Atom.Engine PROPERTIES CXX_STANDARD "20")

target_compile_definitions(Atom.Engine PRIVATE ATOM_EXPORT)

target_link_libraries(Atom.Engine PUBLIC Atom.Core)
target_link_libraries(Atom.Engine PUBLIC Atom.Logging)
target_link_libraries(Atom.Engine PUBLIC glfw)