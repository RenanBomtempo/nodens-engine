workspace "Moxxi"
    architecture "x64"
    startproject "Boids"

    configurations 
    {
        "Debug",
        "Release",
        "Dist"
    }
    

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "Moxxi/vendor/GLFW/include"
IncludeDir["glm"] = "Moxxi/vendor/glm/glm"
IncludeDir["spdlog"] = "Moxxi/vendor/spdlog/include"
IncludeDir["glad"] = "Moxxi/vendor/glad/include"
IncludeDir["imgui"] = "Moxxi/vendor/imgui"

-- Include premake5.lua file from GLFW folder
group "Dependencies"
    include "Moxxi/vendor/GLFW"
    include "Moxxi/vendor/GLAD"
    include "Moxxi/vendor/imgui"
group ""


project "Moxxi"
    location "Moxxi"    
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "mxpch.h"
    pchsource "Moxxi/src/mxpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.imgui}/backends"

    }

    links
    {
        "imgui",
        "GLFW",
        "GLAD",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines 
        {
            "MX_PLATFORM_WINDOWS", 
            "MX_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Boids\"")
        }

    filter "configurations:Debug"
        defines "MX_DEBUG"
        runtime "Debug"
        symbols "On"
        
    filter "configurations:Release"
        defines "MX_RELEASE"
        runtime "Release"
        optimize "On"
    
    filter "configurations:Dist"
        defines "MX_DIST"
        runtime "Release"
        optimize "On"


project "Boids"
    location "Boids"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Moxxi/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.imgui}"
    }

    links 
    {
        "Moxxi"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines 
        {
            "MX_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "MX_DEBUG"
        runtime "Debug"
        symbols "On"
        
    filter "configurations:Release"
        defines "MX_RELEASE"
        runtime "Release"
        optimize "On"
    
    filter "configurations:Dist"
        defines "MX_DIST"
        runtime "Release"
        optimize "On"