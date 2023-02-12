workspace "Atom"
    architecture "x64"
    configurations
    {
        "Debug", "Release"
    }

    location "Build/"

    require "ThirdParty/Premake-Modules/premake-export-compile-commands/export-compile-commands"

    include "Source/Engine/premake.lua"
    include "Source/Sandbox/premake.lua"