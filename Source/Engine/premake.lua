project "AtomEngine"
    kind "SharedLib"
    language "C++"

    location ("Build/".._ACTION)
    targetdir "Build/Bin"
    objdir "Build/Bin-Int"

    files
    {
        "**.h",
        "**.cpp"
    }

    defines
    {
        "ATOM_EXPORT"
    }

    filter "configurations:Debug"
        defines "ATOM_CONFIG_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "ATOM_CONFIG_RELEASE"
        optimize "On"