#include "GBCodeGenerator.h"

namespace GB
{
	CodeGenerator::CodeGenerator() : 
		m_InputFolder(),
		m_OutputFolder(),
		m_Extensions(),
		m_HeaderFileReflectionMacro(),
		m_VariableMacro(),
		m_FunctionMacro(),
		m_CurrentOutputObjectData(),
		m_CurrentOutputFilepath(),
		m_CurrentOutputHeaderFileInclude()
	{}

	CodeGenerator::~CodeGenerator()
	{}

	void CodeGenerator::SetInputFolder(const fs::path& inputFolder)
	{
		m_InputFolder = inputFolder;
		std::cout << "Generator using input folder: " << m_InputFolder << std::endl;
	}

	void CodeGenerator::SetOutputFolder(const fs::path& outputFolder)
	{
		m_OutputFolder = outputFolder;
		std::cout << "Generator using output folder: " << m_OutputFolder << std::endl;
	}

	void CodeGenerator::AddFileExtensionToSearchFor(const std::string& extension)
	{
		m_Extensions.push_back(extension);
	}

	void CodeGenerator::SetHeaderFileReflectionMacroToLookFor(const std::string& macro)
	{
		m_HeaderFileReflectionMacro = macro;
	}

	void CodeGenerator::SetVariableMacroToLookFor(const std::string& macro)
	{
		m_VariableMacro = macro;
	}

	void CodeGenerator::SetVariableMacroEditFlagToLookFor(const std::string& flag)
	{
		m_VariableMacroEditFlag = flag;
	}

	void CodeGenerator::SetFunctionMacroToLookFor(const std::string& macro)
	{
		m_FunctionMacro = macro;
	}

	int CodeGenerator::Generate()
	{
		int result = 0;
		if (!IsValid())
		{
			result = -1;
			return result;
		}

		for (auto& file : fs::recursive_directory_iterator(m_InputFolder))
		{
			bool validExtension = false;
			for (const std::string& extension : m_Extensions)
			{
				if (file.path().extension() == extension)
				{
					validExtension = true;
					break;
				}
			}

			if (validExtension)
			{
				std::string generatedCode;

				if (ProcessInputFile(file.path()))
				{
					std::error_code err;
					if (CreateDirectoryRecursive(m_CurrentOutputFilepath.parent_path().string(), err))
					{
						if (ProcessOutputFile(m_CurrentOutputFilepath))
						{
							// Finished processing the input/output files, so we can safely clear this output data now.
							// Need to empty it otherwise classes, variables and functions will be reflected in other generated files as well.
							m_CurrentOutputObjectData.clear();
						}
					}
					else
					{
						std::cout << "Error: Failed to create directory: " << m_CurrentOutputFilepath << " because " << err << std::endl;
					}
				}
			}
		}

		return result;
	}

	bool CodeGenerator::IsValid()
	{
		bool isValid = true;
		std::string errorMessage;
		if (m_InputFolder.empty())
		{
			errorMessage.append("Error: CodeGenerator doesn't have an input folder, make sure SetInputFolder is called with a valid input folder path!\n");
		}
		if (m_OutputFolder.empty())
		{
			errorMessage.append("Error: CodeGenerator doesn't have an output folder, make sure SetOutputFolder is called with a valid output folder path!\n");
		}
		if (m_Extensions.empty())
		{
			errorMessage.append("Error: CodeGenerator doesn't any file extensions to search, make sure AddFileExtensionToSearchFor is called with a valid file extension (e.g. .h, .hpp)\n");
		}
		if (m_HeaderFileReflectionMacro.empty())
		{
			errorMessage.append("Error: CodeGenerator doesn't have a reflection macro to search for, make sure SetHeaderFileReflectionMacroToLookFor is called with a valid reflection macro\n");
		}
		if (m_VariableMacro.empty())
		{
			errorMessage.append("Error: CodeGenerator doesn't have a variable macro to search for, make sure SetVariableMacroToLookFor is called with a valid variable macro\n");
		}
		if (m_FunctionMacro.empty())
		{
			errorMessage.append("Error: CodeGenerator doesn't have a function macro to search for, make sure SetFunctionMacroToLookFor is called with a valid function macro\n");
		}
		isValid = errorMessage.empty();
		assert(isValid && errorMessage.c_str());
		return isValid;
	}

	bool CodeGenerator::ProcessInputFile(const fs::path& filepath)
	{
		bool result = false;
		std::ifstream file(filepath);

		if (file.is_open())
		{
			std::map<std::string, bool> reflectedClassPrintedEndStatement;
			std::string line;
			std::string currentObjectName;
			bool foundVariableToReflect = false;
			bool foundFunctionToReflect = false;
			bool startOfFirstObjectDetected = false;
			bool printedEndStatement = false;

			// Set what the header file include should be
			const std::string& inputFilepath = filepath.string();
			const std::string& sourceKeyword = "Source";
			const size_t startOfSourceFolder = inputFilepath.find(sourceKeyword) + sourceKeyword.size() + 1;
			const size_t endOfFolders = inputFilepath.size();
			const size_t includePathLength = endOfFolders - startOfSourceFolder;
			m_CurrentOutputHeaderFileInclude = inputFilepath.substr(startOfSourceFolder, includePathLength);

			auto hasPrintedEndStatementFunc = [&](const std::string& name) -> bool
			{
				if (reflectedClassPrintedEndStatement.find(currentObjectName) != reflectedClassPrintedEndStatement.end())
				{
					if (reflectedClassPrintedEndStatement.at(currentObjectName))
					{
						return true;
					}
				}

				return false;
			};

			auto printEndStatementFunc = [&](const std::string& name)
			{
				if (!hasPrintedEndStatementFunc(name))
				{
					std::cout << "End " << name << std::endl;
					reflectedClassPrintedEndStatement.at(name) = true;
				}
			};

			// Go through the input file line by line.
			while (std::getline(file, line))
			{
				// Class/struct name.
				const bool isLineWhereClassIsDeclared = (line.find("class") != std::string::npos && line.find("template") == std::string::npos);
				const bool isLineWhereStructIsDeclared = (line.find("struct") != std::string::npos);
				if (isLineWhereClassIsDeclared || isLineWhereStructIsDeclared)
				{
					// If we already know that this file has some valid reflected class/struct.
					// Then print the end statement for the previous object.
					if (result)
					{
						if (!currentObjectName.empty())
						{
							printEndStatementFunc(currentObjectName);
						}
					}

					const std::string& objectKeywordDeclaration = isLineWhereClassIsDeclared ? "class " : "struct ";
					const size_t startOfObjectName = line.find(objectKeywordDeclaration) + objectKeywordDeclaration.size();
					const size_t colonAfterObjectName = line.find(" : ");
					const bool hasColonAfterObjectName = (colonAfterObjectName != std::string::npos);
					const size_t endOfObjectName = hasColonAfterObjectName ? colonAfterObjectName : line.size();
					const size_t objectNameLength = endOfObjectName - startOfObjectName;
					currentObjectName = line.substr(startOfObjectName, objectNameLength);
					reflectedClassPrintedEndStatement.emplace(std::pair<std::string, bool>{currentObjectName, false });
				}

				// GB_REFLECT
				if (line.find(m_HeaderFileReflectionMacro) != std::string::npos)
				{
					if (!startOfFirstObjectDetected)
					{
						std::cout << "==================================================================" << std::endl;
						startOfFirstObjectDetected = true;
					}

					// This class should be reflected, so therefore we need to process this input file and generate a corresponding output file.
					result = true;
					m_CurrentOutputObjectData.push_back(ObjectData(currentObjectName));

					std::cout << "Start " << currentObjectName << std::endl;
				}

				// GBVARIABLE
				if (line.find(m_VariableMacro) != std::string::npos)
				{
					// Edit
					if (line.find(m_VariableMacroEditFlag) != std::string::npos)
					{
						// This variable needs to be reflected
						// So on the next line we can process it since the macro was found
						foundVariableToReflect = true;
					}
				}
				// Typically, if foundVariableToReflect is true, this next line will always be a variable
				else
				{
					if (foundVariableToReflect)
					{
						const size_t startOfVariableName = line.find(" ") + 1;
						const size_t semiColonAfterVariableName = line.find(";");
						const size_t variableNameLength = semiColonAfterVariableName - startOfVariableName;

						const std::string variableName = line.substr(startOfVariableName, variableNameLength);
						std::cout << "\tVariable: " << variableName << std::endl;

						for (auto& objectData : m_CurrentOutputObjectData)
						{
							if (objectData.m_ObjectName == currentObjectName)
							{
								objectData.m_Variables.push_back(variableName);
							}
						}

						foundVariableToReflect = false;
					}
				}

				// GBFUNCTION
				if (line.find(m_FunctionMacro) != std::string::npos)
				{
					// Same as the variable, will handle it on the next line down
					foundFunctionToReflect = true;
				}
				else
				{
					if (foundFunctionToReflect)
					{
						const size_t startOfFunctionName = line.find(" ") + 1;
						const size_t firstBracketAfterFunctionName = line.find("(");
						const size_t functionNameLength = firstBracketAfterFunctionName - startOfFunctionName;

						const std::string functionName = line.substr(startOfFunctionName, functionNameLength);
						std::cout << "\tFunction: " << functionName << std::endl;

						// TODO: Parameters?

						for (auto& objectData : m_CurrentOutputObjectData)
						{
							if (objectData.m_ObjectName == currentObjectName)
							{
								objectData.m_Functions.push_back(functionName);
							}
						}

						foundFunctionToReflect = false;
					}
				}
			}

			if (result)
			{
				printEndStatementFunc(currentObjectName);
			}

			file.close();
		}

		if (result)
		{
			const std::string& outputPath = m_OutputFolder.string() + filepath.stem().string() + GB::AUTO_GENERATED_FILE_SUFFIX;
			m_CurrentOutputFilepath = fs::path(outputPath);

			std::cout << "Input file: " << filepath.string() << " -> Output file: " << m_CurrentOutputFilepath.string() << std::endl;
			std::cout << "==================================================================" << std::endl << std::endl;
		}

		return result;
	}

	bool CodeGenerator::ProcessOutputFile(const fs::path& filepath)
	{
		bool result = false;
		std::ofstream file(filepath);

		if (file.is_open())
		{
			// Remove any of the generated code that's already there since we are regenerating it now.
			file.clear();

			// Header files.
			std::string generatedCode("");

			// #include "ObjectFolder/ObjectHeaderFile.h"
			const std::string& headerFileInclude = "#include \"" + m_CurrentOutputHeaderFileInclude + "\"\n";

			generatedCode.append("#include \"gbpch.h\"\n");
			generatedCode.append(headerFileInclude);
			generatedCode.append("#include <GBReflect.h>\n\n");
			generatedCode.append("namespace GB\n");
			generatedCode.append("{\n");

			// Looping over every single reflected object found.
			for (size_t iO = 0; iO < m_CurrentOutputObjectData.size(); ++iO)
			{
				ObjectData& objectData = m_CurrentOutputObjectData[iO];

				// GB_REFLECT_BEGIN(ClassName)
				generatedCode.append("\tGB_REFLECT_BEGIN("); generatedCode.append(objectData.m_ObjectName); generatedCode.append(")\n");
				
				// GB_REFLECT_MEMBER(m_VariableName)
				for (auto& variableName : objectData.m_Variables)
				{
					generatedCode.append("\tGB_REFLECT_MEMBER("); generatedCode.append(variableName); generatedCode.append(")\n");
				}

				// GB_REFLECT_FUNCTION(FunctionName)
				for (auto& functionName : objectData.m_Functions)
				{
					generatedCode.append("\tGB_REFLECT_FUNCTION("); generatedCode.append(functionName); generatedCode.append(")\n");
				}
			
				generatedCode.append("\tGB_REFLECT_END()\n");

				// Append another new line to help separate multiple class/struct definitions.
				if (m_CurrentOutputObjectData.size() > 1 && iO < m_CurrentOutputObjectData.size() - 1)
				{
					generatedCode.append("\n");
				}
			}

			generatedCode.append("}\n");

			file << generatedCode;
			file.close();

			result = true;
		}

		return result;
	}
}
