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
IncludeDir["glm"] = "Moxxi/vendor/glm"
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
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glm}"

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
        "Moxxi/vendor/spdlog/include",
        "Moxxi/src",
        "Moxxi/vendor",
        "%{IncludeDir.glm}"
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