#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

using namespace std::chrono_literals;
namespace fs = std::filesystem;

#ifdef _WIN32
#include <windows.h>
#elif (__linux__)
#include <linux/limits.h>
#include <unistd.h>
#else
#include <unistd.h>
#endif

fs::path GetExeDirectory();

namespace Generator
{
	static const std::string GB_REFLECT_INCLUDE = "#include <GBReflect.h>";
	static const std::string GB_REFLECT_BEGIN_MACRO = "GB_REFLECT_BEGIN";
	static const std::string GB_REFLECT_MEMBER_MACRO = "GB_REFLECT_MEMBER";
	static const std::string GB_REFLECT_END_MACRO = "GB_REFLECT_END";
}

fs::path GetExeDirectory()
{
#ifdef _WIN32
	// Windows specific
	wchar_t szPath[MAX_PATH];
	GetModuleFileNameW(NULL, szPath, MAX_PATH);
#else
	// Linux specific
	char szPath[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", szPath, PATH_MAX);
	if (count < 0 || count >= PATH_MAX)
		return {}; // some error
	szPath[count] = '\0';
#endif
	return std::filesystem::path{ szPath }.parent_path() / ""; // to finish the folder path with (back)slash
}

int main(int argc, char** argv)
{
	const fs::path exeFolder = GetExeDirectory();
	std::cout << "Exe path is: " << exeFolder.string() << std::endl;
#if _WIN32
	const std::string& gbHeaderToolFilepath = exeFolder.string() + "/../GBHeaderTool/GBHeaderTool.exe";
#else
	const std::string& gbHeaderToolFilepath = exeFolder.string() + "/../GBHeaderTool/GBHeaderTool";
#endif
	const std::string& reflectionTestFilesSourceFolder = exeFolder.string() + "../../../Sandbox/Source/";
	const std::string& inputFolder = reflectionTestFilesSourceFolder + "ReflectionTestFiles/AutoTestFolderToUse/";
	const std::string& outputFolder = reflectionTestFilesSourceFolder + "GeneratedCode/";
	const std::string& generatedCodeClassTestFilepath = outputFolder + "TestClassReflection_gen.cpp";
	const std::string& generatedCodeStructTestFilepath = outputFolder + "TestStructReflection_gen.cpp";

	// ../Binaries/Debug-windows-x86_64/GBHeaderTool/GBHeaderTool.exe ../Sandbox/Source/ReflectionTestFiles/AutoTestFolderToUse/ ../Sandbox/Source/GeneratedCode/
#if _WIN32
	std::string runCodeGeneratorCommand = "start " + gbHeaderToolFilepath + " "	+ inputFolder + " "	+ outputFolder;
#else
	std::string runCodeGeneratorCommand = gbHeaderToolFilepath + " " + inputFolder + " " + outputFolder;
#endif
	int codeGeneratorReturnCode = std::system(runCodeGeneratorCommand.c_str());
	std::this_thread::sleep_for(5000ms);
	
	if (codeGeneratorReturnCode != 0)
	{
		std::cout << "Error: Code generator failed - see output for possible reasons!" << std::endl;
		return -1;
	}

	bool bFoundClassGBReflectInclude = false;
	bool bFoundClassGBReflectBeginMacro = false;
	bool bFoundClassGBReflectMemberMacro = false;
	bool bFoundClassGBReflectEndMacro = false;

	bool bFoundStructGBReflectInclude = false;
	bool bFoundStructGBReflectBeginMacro = false;
	bool bFoundStructGBReflectMemberMacro = false;
	bool bFoundStructGBReflectEndMacro = false;

	enum class FileType
	{
		Class,
		Struct
	};

	auto checkFileForMacroFunc = [&](std::ifstream& file, const FileType type)
	{
		std::string line = "";

		while (std::getline(file, line))
		{
			// "#include <GBReflect.h>"
			if (line.find(Generator::GB_REFLECT_INCLUDE) != std::string::npos)
			{
				if (type == FileType::Class)
				{
					bFoundClassGBReflectInclude = true;
				}
				else if (type == FileType::Struct)
				{
					bFoundStructGBReflectInclude = true;
				}
			}

			// GB_REFLECT_BEGIN
			if (line.find(Generator::GB_REFLECT_BEGIN_MACRO) != std::string::npos)
			{
				if (type == FileType::Class)
				{
					bFoundClassGBReflectBeginMacro = true;
				}
				else if (type == FileType::Struct)
				{
					bFoundStructGBReflectBeginMacro = true;
				}
			}

			// GB_REFLECT_MEMBER
			if (line.find(Generator::GB_REFLECT_MEMBER_MACRO) != std::string::npos)
			{
				if (type == FileType::Class)
				{
					bFoundClassGBReflectMemberMacro = true;
				}
				else if (type == FileType::Struct)
				{
					bFoundStructGBReflectMemberMacro = true;
				}
			}

			// GB_REFLECT_END
			if (line.find(Generator::GB_REFLECT_END_MACRO) != std::string::npos)
			{
				if (type == FileType::Class)
				{
					bFoundClassGBReflectEndMacro = true;
				}
				else if (type == FileType::Struct)
				{
					bFoundStructGBReflectEndMacro = true;
				}
			}
		}
	};

	std::ifstream ifStreamClassFile(generatedCodeClassTestFilepath);
	if (ifStreamClassFile.is_open())
	{
		checkFileForMacroFunc(ifStreamClassFile, FileType::Class);
	}

	std::ifstream ifStreamStructFile(generatedCodeStructTestFilepath);
	if (ifStreamStructFile.is_open())
	{
		checkFileForMacroFunc(ifStreamStructFile, FileType::Struct);
	}

	std::string errorMessage = "";
	auto checkForAnyErrorsFunc = [&](bool valueToCheck, const std::string& stringThatTestFailedToFind, const FileType fileType)
	{
		if (!valueToCheck)
		{
			errorMessage.append("Didn't find " + stringThatTestFailedToFind + " in " + (fileType == FileType::Class ? generatedCodeClassTestFilepath : generatedCodeStructTestFilepath) + "\n");
		}
	};

	checkForAnyErrorsFunc(bFoundClassGBReflectInclude,		Generator::GB_REFLECT_INCLUDE,		FileType::Class);
	checkForAnyErrorsFunc(bFoundClassGBReflectBeginMacro,	Generator::GB_REFLECT_BEGIN_MACRO,	FileType::Class);
	checkForAnyErrorsFunc(bFoundClassGBReflectMemberMacro,	Generator::GB_REFLECT_MEMBER_MACRO, FileType::Class);
	checkForAnyErrorsFunc(bFoundClassGBReflectEndMacro,		Generator::GB_REFLECT_END_MACRO,	FileType::Class);

	checkForAnyErrorsFunc(bFoundStructGBReflectInclude,		Generator::GB_REFLECT_INCLUDE,		FileType::Struct);
	checkForAnyErrorsFunc(bFoundStructGBReflectBeginMacro,	Generator::GB_REFLECT_BEGIN_MACRO,	FileType::Struct);
	checkForAnyErrorsFunc(bFoundStructGBReflectMemberMacro, Generator::GB_REFLECT_MEMBER_MACRO, FileType::Struct);
	checkForAnyErrorsFunc(bFoundStructGBReflectEndMacro,	Generator::GB_REFLECT_END_MACRO,	FileType::Struct);

	if (!errorMessage.empty())
	{
		std::cout << "Error - GBHeaderToolTest Failed:\n" << errorMessage << std::endl;
		return -1;
	}

	std::cout << "GBHeaderToolTest succeeded!" << std::endl;
	return 0;
}
