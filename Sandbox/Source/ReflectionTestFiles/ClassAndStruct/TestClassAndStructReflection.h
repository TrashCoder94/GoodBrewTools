#pragma once

#include "BaseClassAndStruct.h"

namespace GB
{
	struct TestStructDefinedInSameFileAsClass : public BaseStructDefinedInSameFileAsBaseClass
	{
		GB_REFLECT()

	public:
		TestStructDefinedInSameFileAsClass();
		~TestStructDefinedInSameFileAsClass();

		GBVARIABLE(Edit)
		std::string m_TestStructDefinedInSameFileAsClassVariableString;

		GBVARIABLE(Edit)
		float m_TestStructDefinedInSameFileAsClassVariableFloat;

		GBVARIABLE(Edit)
		int m_TestStructDefinedInSameFileAsClassVariableInt;

		GBVARIABLE(Edit)
		bool m_TestStructDefinedInSameFileAsClassVariableBool;

		GBFUNCTION()
		void TestStructDefinedInSameFileAsClassFunction();
	};

	class TestClassDefinedInSameFileAsStruct : public BaseClassDefinedInSameFileAsBaseStruct
	{
		GB_REFLECT()

	public:
		TestClassDefinedInSameFileAsStruct();
		~TestClassDefinedInSameFileAsStruct();

		GBVARIABLE(Edit)
		std::string m_TestClassDefinedInSameFileAsStructVariableString;

		GBVARIABLE(Edit)
		float m_TestClassDefinedInSameFileAsStructVariableFloat;

		GBVARIABLE(Edit)
		int m_TestClassDefinedInSameFileAsStructVariableInt;

		GBVARIABLE(Edit)
		bool m_TestClassDefinedInSameFileAsStructVariableBool;

		GBFUNCTION()
		void TestClassDefinedInSameFileAsStructFunction();
	};
}