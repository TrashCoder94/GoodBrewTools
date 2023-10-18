project "CommitChecker"
	kind "WindowedApp"
	language "C#"
	cppdialect "C++17"
	flags { "WPF" }

	targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Source/**.cs",
		"Source/**.xaml",
		"Source/Assets/Images/**.png"
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
	
	filter "files:Source/Assets/Images/**.png"
		buildaction "Resource"
	filter {}
	
	copyCommitCheckerAssetsToBinaryFolder()
	copyCommitCheckerScriptsToBinaryFolder()
