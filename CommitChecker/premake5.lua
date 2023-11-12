project "CommitChecker"
	kind "WindowedApp"
	language "C#"
	cppdialect "C++17"
	flags { "WPF" }
	icon "Source/Assets/Images/CommitCheckerIcon.ico"

	targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Source/**.cs",
		"Source/**.xaml",
		"Source/Assets/Audio/**",
		"Source/Assets/Images/**"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"Source"
	}
	
	links
	{
		"Microsoft.Csharp",
		"PresentationCore",
		"PresentationFramework",
		"System",
		"System.Core",
		"System.Data",
		"System.Data.DataSetExtensions",
		"System.Xaml",
		"System.Xml",
		"System.Xml.Linq",
		"WindowsBase"
	}
	
	filter "files:Source/Assets/Images/**"
		buildaction "Resource"
	filter {}
	
	filter "files:Source/Assets/Audio/**"
		buildaction "Resource"
	filter {}
	
	-- Needed to allow the use of Debug.Assert
	filter { "configurations:Debug" }
		defines
		{
			"DEBUG",
		}
	filter {}
	
	copyCommitCheckerAssetsToBinaryFolder()
	copyCommitCheckerScriptsToBinaryFolder()
