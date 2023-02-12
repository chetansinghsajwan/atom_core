project "AtomEngine"
    kind "SharedLib"
    language "C++"

    location "Build/"
    targetdir "Build/Bin"
    objdir "Build/Bin-Int"

    files
    {
        "**.h",
        "**.cpp"
    }

    defines
    {
        "ATOM_EXPORT",
        "ATOM_PLATFORM_LINUX"
    }

    includedirs
    {
        "Private/",
        "Public/"
    }

    filter "configurations:Debug"
        defines "ATOM_CONFIG_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "ATOM_CONFIG_RELEASE"
        optimize "On"