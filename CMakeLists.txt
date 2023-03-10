cmake_minimum_required(VERSION 3.0.0)
project(AtomEngine VERSION 0.1.0 LANGUAGES CXX C)

enable_testing()

add_subdirectory(ThirdParty/Fmt/fmt)
add_subdirectory(ThirdParty/Catch2/Catch2)
add_subdirectory(ThirdParty/GLFW/glfw)
add_subdirectory(ThirdParty/WjCryptLib)

add_subdirectory(Source/Core)
add_subdirectory(Source/Logging)
add_subdirectory(Source/Engine)
add_subdirectory(Source/Sandbox)