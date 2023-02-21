workspace "GraphicsMath"
    architecture "x86_64"
    startproject "GraphicsMath"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["ImGui"]     = "GraphicsMath/vendor/imgui"

include "GraphicsMath/vendor/imgui"

project "GraphicsMath"
    location "GraphicsMath"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.ImGui}",
    }

    links
    {
        "imgui",
        "d3d11"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "GM_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "GM_RELEASE"
        runtime "Release"
        optimize "on"