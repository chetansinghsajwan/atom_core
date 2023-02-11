project "Sandbox"
    kind "ConsoleApp"
    language "C++"

    location ("Build/".._ACTION)
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