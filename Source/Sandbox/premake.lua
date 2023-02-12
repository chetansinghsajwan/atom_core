project "Sandbox"
    kind "ConsoleApp"
    language "C++"

    location "Build/"
    targetdir "Build/Bin"
    objdir "Build/Bin-Int"

    files
    {
        "**.h",
        "**.cpp"
    }

    links
    {
        "AtomEngine"
    }

    includedirs
    {
        "Private/",
        "Public/",
        "../Engine/Public/"
    }