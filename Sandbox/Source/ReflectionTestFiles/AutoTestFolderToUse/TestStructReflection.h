#pragma once

#include "BaseStruct.h"

namespace GB
{
	struct TestStructReflection : public BaseStruct
	{
		GB_REFLECT()

	public:
		TestStructReflection();
		~TestStructReflection();

		GBVARIABLE(Edit)
		std::string m_TestStructVariableString;
	
		GBVARIABLE(Edit)
		float m_TestStructVariableFloat;

		GBVARIABLE(Edit)
		int m_TestStructVariableInt;

		GBVARIABLE(Edit)
		bool m_TestStructVariableBool;

		GBFUNCTION()
		void TestStructFunction();
	};
}
