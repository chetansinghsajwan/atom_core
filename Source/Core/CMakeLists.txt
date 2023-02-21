# --------------------------------------------------------------------------------------------------
# Atom.Core
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE SOURCES "Private/**.cpp"
    "Private/**.h" "Public/**.h")

add_library(Atom.Core SHARED ${SOURCES})
target_include_directories(Atom.Core PRIVATE "Private/")
target_include_directories(Atom.Core PUBLIC "Public/")
set_target_properties(Atom.Core PROPERTIES CXX_STANDARD "20")

target_compile_definitions(Atom.Core PRIVATE ATOM_EXPORT)
target_compile_definitions(Atom.Core PUBLIC ATOM_PLATFORM_LINUX)