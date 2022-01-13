workspace "Moxxi"
    architecture "x64"

    configurations 
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Moxxi"
    location "Moxxi"    
    kind "SharedLib"
    language "C++"

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
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines 
        {
            "MX_PLATFORM_WINDOWS", 
            "MX_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Boids")
        }

    filter "configurations:Debug"
        defines "MX_DEBUG"
        symbols "On"
        
    filter "configurations:Release"
        defines "MX_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "MX_DIST"
        optimize "On"

project "Boids"
    location "Boids"
    kind "ConsoleApp"
    language "C++"

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
        "Moxxi/src"
    }

    links 
    {
        "Moxxi"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines 
        {
            "MX_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "MX_DEBUG"
        symbols "On"
        
    filter "configurations:Release"
        defines "MX_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "MX_DIST"
        optimize "On"