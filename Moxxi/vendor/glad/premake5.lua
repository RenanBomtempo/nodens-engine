project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }
    
    includedirs
    {
        "include"
    }

	filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines "MX_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "MX_RELEASE"
        runtime "Release"
        optimize "on"