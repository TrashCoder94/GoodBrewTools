#pragma once

#include "BaseClass.h"

namespace GB
{
	class TestClassAReflection : public BaseClass
	{
		GB_REFLECT()

	public:
		TestClassAReflection();
		~TestClassAReflection();

		GBVARIABLE(Edit)
		std::string m_TestClassAVariableString;

		GBVARIABLE(Edit)
		float m_TestClassAVariableFloat;

		GBVARIABLE(Edit)
		int m_TestClassAVariableInt;

		GBVARIABLE(Edit)
		bool m_TestClassAVariableBool;

		GBFUNCTION()
		void TestClassAFunction();
	};

	class TestClassBReflection : public BaseClass
	{
		GB_REFLECT()

	public:
		TestClassBReflection();
		~TestClassBReflection();

		GBVARIABLE(Edit)
		std::string m_TestClassBVariableString;

		GBVARIABLE(Edit)
		float m_TestClassBVariableFloat;

		GBVARIABLE(Edit)
		int m_TestClassBVariableInt;

		GBVARIABLE(Edit)
		bool m_TestClassBVariableBool;

		GBFUNCTION()
		void TestClassBFunction();
	};
}