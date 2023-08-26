# --------------------------------------------------------------------------------------------------
# Atom.Engine
# --------------------------------------------------------------------------------------------------

find_package(glfw3 CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "Private/**.cpp"
    "Private/**.h" "Public/**.h")

add_library(Atom.Engine STATIC ${SOURCES})
target_include_directories(Atom.Engine PRIVATE "Private/")
target_include_directories(Atom.Engine PUBLIC "Public/")

target_compile_definitions(Atom.Engine PRIVATE ATOM_EXPORT)

target_link_libraries(Atom.Engine PUBLIC Atom.Core)
target_link_libraries(Atom.Engine PUBLIC Atom.Logging)
target_link_libraries(Atom.Engine PUBLIC glfw)

# --------------------------------------------------------------------------------------------------
# Atom.Engine.Sandbox
# --------------------------------------------------------------------------------------------------

add_executable(Atom.Engine.Sandbox "Sandbox/Sandbox.cpp")
target_include_directories(Atom.Engine.Sandbox PRIVATE "Sandbox")
target_link_libraries(Atom.Engine.Sandbox PUBLIC Atom.Engine)