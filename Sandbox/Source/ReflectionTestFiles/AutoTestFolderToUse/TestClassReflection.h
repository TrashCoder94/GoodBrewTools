#pragma once

#include "BaseClass.h"

namespace GB
{
	class TestClassReflection : public BaseClass
	{
		GB_REFLECT()

	public:
		TestClassReflection();
		~TestClassReflection();

		GBVARIABLE(Edit)
		std::string m_TestClassVariableString;

		GBVARIABLE(Edit)
		float m_TestClassVariableFloat;

		GBVARIABLE(Edit)
		int m_TestClassVariableInt;

		GBVARIABLE(Edit)
		bool m_TestClassVariableBool;

		GBFUNCTION()
		void TestClassFunction();
	};
}