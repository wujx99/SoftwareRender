workspace "SoftwareRender"
	architecture "x64"
	
	startproject "SoftwareRender"
	configurations
	{
		"Debug",
		"Release",
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}



project "SoftwareRender"
	location "SoftwareRender"
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
		"vendor/spdlog/include",
		"src",
	}

	

	filter "system:windows"
		
		systemversion "latest"

		defines
		{
			"SR_PLATFORM_WINDOWS",
		}

		
	filter "configurations:Debug"
		defines "SR_DEBUG"
		runtime "Debug"
		symbols "on"
		optimize "off"
		
	filter "configurations:Release"
		defines "SR_RELEASE"
		runtime "Release"
		optimize "off"