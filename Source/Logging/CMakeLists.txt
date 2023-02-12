# --------------------------------------------------------------------------------------------------
# Atom.Logging
# --------------------------------------------------------------------------------------------------

set(SOURCES "Private/Logging.cpp")

add_library(Atom.Logging ${SOURCES})
target_include_directories(Atom.Logging PRIVATE "Private/")
target_include_directories(Atom.Logging PUBLIC "Public/")
target_include_directories(Atom.Logging PUBLIC "../../ThirdParty/Fmt/fmt/include/")
set_target_properties(Atom.Logging PROPERTIES CXX_STANDARD "20")