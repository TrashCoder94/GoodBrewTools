#include "GBCodeGenerator/GBCodeGenerator.h"

// Test command arguments
// InputFolder:																	 OutputFolder:
// C:\SideProjects\Repositories\GoodBrewTools\Sandbox\Source\ReflectionTestFiles C:\SideProjects\Repositories\GoodBrewTools\Sandbox\Source\GeneratedCode

int main(int argc, char** argv)
{
	int result = 0;
	result = argc < 3 ? -1 : 0;
	assert(result == 0 && "Error: not enough arguments to run code generator. Please provide input and output folder as command arguments to fix this.");
	if (result == -1)
	{
		return result;
	}

	const fs::path& exeFolder = GB::GetExeDirectory();
	result = exeFolder.empty() ? -1 : 0;
	assert(result == 0 && "Error: exeFolder doesn't exist? Check logic in GB::GetExeDirectory() to fix this");
	if (result == -1)
	{
		return result;
	}

	std::cout << "==================================================================" << std::endl;
	std::cout << "Code Generator" << std::endl;
	std::cout << "==============" << std::endl;
	std::cout << "Exe path is: " << exeFolder.string() << std::endl;

	// Setting up the generator with input (original source files to search) and output (where to put the auto generated files) folders
	// E.g. Input Folder = .../Source/... and Output Folder = .../Source/GeneratedCode/...
	GB::CodeGenerator codeGenerator;
	const fs::path& inputFolder = fs::path(argv[1]);
	const fs::path& outputFolder = fs::path(argv[2]); // TODO: Do I need to create this if it doesn't exist yet?
	codeGenerator.SetInputFolder(inputFolder);
	codeGenerator.SetOutputFolder(outputFolder);

	// Set what file extensions to search for when looking for code macros to auto generate files from
	codeGenerator.AddFileExtensionToSearchFor(".h");
	codeGenerator.AddFileExtensionToSearchFor(".hpp");

	// Set what macros to look for when searching the source files
	
	// This will search for GB_REFLECT in the header files
	// This will decide whether to generate any cpp file at all
	codeGenerator.SetHeaderFileReflectionMacroToLookFor(GB::MACRO_REFLECT);

	// This will search for GBVARIABLE in the header files and is what will expose variables to the editor.
	// They should be marked with EVariableFlags::Edit to expose them, ::Hidden if you don't want to see them in the editor.
	// E.g. 
	// GBVARIABLE(EVariableFlags::Edit)
	// float m_FloatToShowInEditor = 0.0f;
	codeGenerator.SetVariableMacroToLookFor(GB::MACRO_VARIABLE);
	codeGenerator.SetVariableMacroToLookFor(GB::MACRO_VARIABLE_FLAG_EDIT);

	// This will search for GBFUNCTION in the header files.
	// TODO: Give example usage...
	codeGenerator.SetFunctionMacroToLookFor(GB::MACRO_FUNCTION);
	
	std::cout << "==================================================================" << std::endl << std::endl;

	result = codeGenerator.Generate();
	assert(result == 0 && "CodeGenerator::Generate failed!");
	system("pause");

	return result;
}
