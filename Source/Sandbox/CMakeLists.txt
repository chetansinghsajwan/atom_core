# --------------------------------------------------------------------------------------------------
# Sandbox
# --------------------------------------------------------------------------------------------------

set(SOURCES "Private/Sandbox.cpp")

add_executable(Sandbox ${SOURCES})
target_include_directories(Sandbox PRIVATE "Private/")
target_include_directories(Sandbox PUBLIC "Public/")

target_link_libraries(Sandbox PUBLIC Atom.Engine)