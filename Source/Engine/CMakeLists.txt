# --------------------------------------------------------------------------------------------------
# Atom.Engine
# --------------------------------------------------------------------------------------------------

set(SOURCES "Private/Main.cpp"
    "Private/Application.cpp")

add_library(Atom.Engine ${SOURCES})
target_include_directories(Atom.Engine PRIVATE "Private/")
target_include_directories(Atom.Engine PUBLIC "Public/")
set_target_properties(Atom.Engine PROPERTIES CXX_STANDARD "20")