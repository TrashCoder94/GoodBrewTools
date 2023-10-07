#pragma once

#include "GBCodeGeneratorUtilities.h"
#include "GBReflection/GBReflect.h"

namespace GB
{
	class CodeGenerator
	{
	public:
		CodeGenerator();
		~CodeGenerator();

		void SetInputFolder(const fs::path& inputFolder);
		void SetOutputFolder(const fs::path& outputFolder);

		void AddFileExtensionToSearchFor(const std::string& extension);

		void SetHeaderFileReflectionMacroToLookFor(const std::string& macro);
		void SetVariableMacroToLookFor(const std::string& macro);
		void SetVariableMacroEditFlagToLookFor(const std::string& flag);
		void SetFunctionMacroToLookFor(const std::string& macro);

		int Generate();

	private:
		bool IsValid();

		bool ProcessInputFile(const fs::path& filepath);
		bool ProcessOutputFile(const fs::path& filepath);

		fs::path m_InputFolder;
		fs::path m_OutputFolder;

		std::vector<std::string> m_Extensions;
		std::string m_HeaderFileReflectionMacro;
		std::string m_VariableMacro;
		std::string m_VariableMacroEditFlag;
		std::string m_FunctionMacro;

		struct ObjectData
		{
			ObjectData(const std::string& name) : m_ObjectName(name) {}
			~ObjectData() = default;

			std::string m_ObjectName;
			std::vector<std::string> m_Variables;
			std::vector<std::string> m_Functions;
		};

		std::vector<ObjectData> m_CurrentOutputObjectData;
		fs::path m_CurrentOutputFilepath;
		std::string m_CurrentOutputHeaderFileInclude;
	};
}