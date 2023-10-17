include "./ThirdParty/premake/premake_customization/solution_items.lua"

workspace "GBTools"
	architecture "x86_64"
	
	configurations
	{
		"Debug",
		"Release",
		"Shipping"
	}

	if os.is64bit() and not os.host() == "windows" then
		platforms "x86_64"
	else
		platforms { "x86", "x86_64" }
	end

	if not os.host() == "macosx" then
		solution_items { ".editorconfig" }
	end

	flags
	{
		"MultiProcessorCompile"
	}
	
	linkoptions 
	{ 
		"-IGNORE:4006,4099"
	}
	
	filter { "configurations:Debug" }
		defines
		{
			"GBT_DEBUG",
		}
		runtime "Debug"
		symbols "on"
	filter {}
	filter { "configurations:Release" }
		defines
		{
			"GBT_RELEASE"
		}
		runtime "Release"
		optimize "on"
	filter {}
	filter { "configurations:Shipping" }
		defines
		{
			"GBT_SHIPPING"
		}
		runtime "Release"
		optimize "on"
	filter {}
	
	filter { "platforms:x86" }
		architecture "x86"
	filter {}
	filter { "platforms:x86_64" }
		architecture "x86_64"
	filter {}
	filter { "system:windows" }
		toolset "v143"
		buildoptions { "/Zc:__cplusplus" }
	filter {}
	filter { "system:macosx" }
		xcodebuildsettings {
			["MACOSX_DEPLOYMENT_TARGET"] = "10.9",
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
		};
	filter {}

-- Folder to put Binaries and Intermediate files into
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories
IncludeDir = {}
IncludeDir["imgui"] = "%{wks.location}/ThirdParty/imgui"
IncludeDir["Reflection"] = "%{wks.location}/GBHeaderTool/ThirdParty/Reflection"
IncludeDir["GBReflection"] = "%{wks.location}/GBHeaderTool/Source/GBReflection"
IncludeDir["GBHeaderTool"] = "%{wks.location}/GBHeaderTool/Source"

function copyReflectionFilesAcross()
	filter { "system:windows" }
		postbuildcommands
		{
			"{COPY} %{wks.location}/GBHeaderTool/ThirdParty/Reflection/ReflectCommon.cpp %{prj.location}/ThirdParty/Reflection/"
		}
	filter {}

	filter { "system:not windows" }
		postbuildcommands
		{
			"cp -R %{wks.location}/GBHeaderTool/ThirdParty/Reflection/ReflectCommon.cpp %{prj.location}/ThirdParty/Reflection/"
		}
	filter {}
end

function copyCommitCheckerAssetsToBinaryFolder()
	filter { "system:windows" }
		postbuildcommands
		{
			"{COPY} %{wks.location}/CommitChecker/Source/Assets/ $(TargetDir)/Assets/"
		}
	filter {}
end

function copyCommitCheckerScriptsToBinaryFolder()
	filter { "system:windows" }
		postbuildcommands
		{
			"{COPY} %{wks.location}/Scripts/CommitChecker/ $(TargetDir)"
		}
	filter {}
end

group "Dependencies"
	include "ThirdParty/premake"
group ""

group "GBTools"
if os.host() == "windows" then
	include "CommitChecker"
end
	include "GBHeaderTool"
group ""

group "GBTools/Tests"
	include "Tests/GBHeaderToolTest"
group ""
