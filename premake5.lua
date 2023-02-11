workspace "Atom"
    architecture "x64"
    configurations
    {
        "Debug", "Release"
    }

    location ("Build/".._ACTION)

    include "Source/Engine/premake.lua"
    include "Source/Sandbox/premake.lua"