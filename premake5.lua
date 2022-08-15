workspace "Nodens"
    architecture "x64"
    startproject "ExampleApp"

    configurations 
    {
        "Debug",
        "Release",
        "Dist"
    }
    

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "Nodens/vendor/GLFW/include"
IncludeDir["glm"] = "Nodens/vendor/glm"
IncludeDir["glad"] = "Nodens/vendor/glad/include"
IncludeDir["imgui"] = "Nodens/vendor/imgui"

-- Include premake5.lua file from GLFW folder
group "Dependencies"
    include "Nodens/vendor/GLFW"
    include "Nodens/vendor/GLAD"
    include "Nodens/vendor/imgui"
group ""


project "Nodens"
    location "Nodens"    
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ndpch.h"
    pchsource "Nodens/src/ndpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    defines 
    {
        "_CRT_SECURE_NO_WARNINGS"
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
        systemversion "latest"

        defines 
        {
            "ND_PLATFORM_WINDOWS", 
            "ND_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "ND_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "ND_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "ND_DIST"
        runtime "Release"
        optimize "on"


project "ExampleApp"
    location "ExampleApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Nodens/vendor/spdlog/include",
        "Nodens/src",
        "Nodens/vendor",
        "%{IncludeDir.glm}"
    }

    links 
    {
        "Nodens"
    }

    filter "system:windows"
        systemversion "latest"

        defines 
        {
            "ND_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "ND_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "ND_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "ND_DIST"
        runtime "Release"
        optimize "on"