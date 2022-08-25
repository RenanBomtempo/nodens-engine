workspace "Moxxi"
    architecture "x64"
    startproject "ShallowWaters"

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

-- Solution Folders
group "Dependencies"
    include "Moxxi/vendor/GLFW"
    include "Moxxi/vendor/GLAD"
    include "Moxxi/vendor/imgui"
group ""

-- Engine Project --
project "Moxxi"
    location "Moxxi"    
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
            "MX_PLATFORM_WINDOWS", 
            "MX_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "MX_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "MX_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "MX_DIST"
        runtime "Release"
        optimize "on"
-- End Project Moxxi

group "Applications"
    project "Boids"
        location "Applications/Boids"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.group}/%{prj.name}/src/**.h",
            "%{prj.group}/%{prj.name}/src/**.cpp"
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
            systemversion "latest"

            defines 
            {
                "MX_PLATFORM_WINDOWS",
            }

        filter "configurations:Debug"
            defines "MX_DEBUG"
            runtime "Debug"
            symbols "on"
            
        filter "configurations:Release"
            defines "MX_RELEASE"
            runtime "Release"
            optimize "on"
        
        filter "configurations:Dist"
            defines "MX_DIST"
            runtime "Release"
            optimize "on"
    -- End Project Boids

    project "ShallowWaters"
        location "Applications/ShallowWaters"    
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.group}/%{prj.name}/src/**.h",
            "%{prj.group}/%{prj.name}/src/**.cpp"
        }

        includedirs
        {
            "Plugins/ALG/src",
            "Moxxi/vendor/spdlog/include",
            "Moxxi/src",
            "Moxxi/vendor",
            "%{IncludeDir.glm}"
        }

        links
        {
            "Moxxi",
            "ALG"
        }

        filter "system:windows"
            systemversion "latest"

            defines 
            {
                "MX_PLATFORM_WINDOWS",
                "ALG_PLATFORM_WINDOWS" 
            }

        filter "configurations:Debug"
            defines "ALG_DEBUG"
            runtime "Debug"
            symbols "on"
            
        filter "configurations:Release"
            defines "ALG_RELEASE"
            runtime "Release"
            optimize "on"
        
        filter "configurations:Dist"
            defines "ALG_DIST"
            runtime "Release"
            optimize "on"
    -- End Project ShallowWaters
group "" -- End Group Applications

group "Plugins"
    project "ALG"
        location "Plugins/ALG"    
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "algpch.h"
        pchsource "Plugins/ALG/src/algpch.cpp"

        files
        {
            "%{prj.group}/%{prj.name}/src/**.h",
            "%{prj.group}/%{prj.name}/src/**.cpp"
        }

        includedirs
        {
            "%{prj.group}/%{prj.name}/src",
            "%{prj.group}/%{prj.name}/vendor/spdlog/include"
        }

        filter "system:windows"
            systemversion "latest"

            defines 
            {
                "ALG_PLATFORM_WINDOWS"
            }

        filter "configurations:Debug"
            defines "ALG_DEBUG"
            runtime "Debug"
            symbols "on"
            
        filter "configurations:Release"
            defines "ALG_RELEASE"
            runtime "Release"
            optimize "on"
        
        filter "configurations:Dist"
            defines "ALG_DIST"
            runtime "Release"
            optimize "on"
group "" -- End Group Plugins

group "Utility"
    project "ALG Reference"
        location "Utility/ALG-Reference"    
        kind "Utility"
        language "C++"
        cppdialect "C++17"
    
    files
        {
            "%{prj.group}/%{prj.name}/**.h",
            "%{prj.group}/%{prj.name}/**.cpp"
        }
group "" -- End Group Utility