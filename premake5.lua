workspace "NGame"
--[[ GENERAL CONFIGURATION ]]--
	action = (_ACTION == nil) and "" or _ACTION
	projectPath = "projects/" .. action
	
	configurations { "Debug", "Release" }
	platforms "x64"
	location (projectPath)
	
	targetdir 	 "%{prj.location}/bin/%{prj.name}-%{cfg.buildcfg}"
	objdir 		 "%{prj.location}/obj/%{prj.name}-%{cfg.buildcfg}"	
	
-- Systems
	system "windows"
		systemversion "latest"
		defines 
		{
			"NG_WINDOWS"
		}

-- Configurations
	configuration "Debug"
		runtime "Debug"
		symbols "on"
		defines
		{
			"__ERROR_LOG__",
			"__TRACE_LOG__",
			"NG_DEBUG"
		}
		
	configuration "Release"
		runtime "Release"
		optimize "on"
		flags { "LinkTimeOptimization" }
		defines
		{
			"NG_RELEASE"
		}

-- Actions
	newaction
	{
		trigger = "clean",
		description = "cleaning the solution",
		execute = function()
			error("clean is not implemented yet")
		end
	}
	
-- Variables
	sdk = os.getenv("NGAME_SDK"):gsub("\\", "/")
	
	includes = 
	{
		["boost"] 	= sdk .. "/boost_1_70",
		["gl3w"]	= sdk .. "/gl3w/include",
		["GLFW"]	= sdk .. "/GLFW/include"
	}
	
	libs = 
	{
		["boost"] 	= sdk .. "/boost_1_70/lib64",
		["GLFW"] 	= sdk .. "/GLFW/lib64"
	}
	
	if not sdk then
		error("NGAME_SDK env variable is not defined.")
	end
	
--[[ PROJECTS ]]--
project "Common"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	pchheader "pch.h"
	pchsource "src/Common/pch.cpp"
	
	files
	{
		"src/Common/**.h",
		"src/Common/**.cpp"
	}

	includedirs
	{
		includes["boost"]
	}

project "NServer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	files
	{
		"src/Server/**.h",
		"src/Server/**.cpp"
	}
	
	includedirs
	{
		includes["boost"],
		"src/Common"
	}
	
	libdirs 
	{
		libs["boost"]
	}
	
	links
	{
		"Common"
	}

project "NClient"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	files 
	{
		"src/Client/**.h",
		"src/Client/**.cpp"
	}
	
	includedirs
	{
		includes["boost"],
		includes["gl3w"],
		includes["GLFW"],
		"src/Common"
	}
	
	libdirs 
	{
		libs["boost"],
		libs["GLFW"]
	}
	
	links
	{
		"Common",
		"gl3w",
		"glfw3.lib"
	}
	
--[[ VENDORS ]]--
project "gl3w"
	location (projectPath .. "/vendor")
	targetdir 	 "%{prj.location}/../bin/%{prj.name}-%{cfg.buildcfg}"
	objdir 		 "%{prj.location}/../obj/%{prj.name}-%{cfg.buildcfg}"	
	
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	files
	{
		(includes["gl3w"] .. "/../gl3w.c"),
		(includes["gl3w"] .. "/GL/gl3w.h"),
		(includes["gl3w"] .. "/GL/glcorearb.h"),
		(includes["gl3w"] .. "/KHR/khrplatform.h")
	}

	includedirs
	{
		includes["gl3w"]
	}