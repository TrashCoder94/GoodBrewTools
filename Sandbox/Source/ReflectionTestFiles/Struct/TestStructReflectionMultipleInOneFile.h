#pragma once

#include "BaseStruct.h"

namespace GB
{
	struct TestStructAReflection : public BaseStruct
	{
		GB_REFLECT()

	public:
		TestStructAReflection();
		~TestStructAReflection();

		GBVARIABLE(Edit)
		std::string m_TestStructAVariableString;
	
		GBVARIABLE(Edit)
		float m_TestStructAVariableFloat;

		GBVARIABLE(Edit)
		int m_TestStructAVariableInt;

		GBVARIABLE(Edit)
		bool m_TestStructAVariableBool;

		GBFUNCTION()
		void TestStructAFunction();
	};

	struct TestStructBReflection : public BaseStruct
	{
		GB_REFLECT()

	public:
		TestStructBReflection();
		~TestStructBReflection();

		GBVARIABLE(Edit)
		std::string m_TestStructBVariableString;

		GBVARIABLE(Edit)
		float m_TestStructBVariableFloat;

		GBVARIABLE(Edit)
		int m_TestStructBVariableInt;

		GBVARIABLE(Edit)
		bool m_TestStructBVariableBool;

		GBFUNCTION()
		void TestStructBFunction();
	};
}
