include(CMakeFindDependencyMacro)

find_dependency(fmt REQUIRED)
find_dependency(magic_enum REQUIRED)
find_dependency(Catch2 REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/atom_core-targets.cmake")
