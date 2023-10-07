#include "TestClassAndStructReflection.h"

namespace GB
{
	TestStructDefinedInSameFileAsClass::TestStructDefinedInSameFileAsClass() : m_TestStructDefinedInSameFileAsClassVariableString(),
		m_TestStructDefinedInSameFileAsClassVariableFloat(0.0f),
		m_TestStructDefinedInSameFileAsClassVariableInt(0),
		m_TestStructDefinedInSameFileAsClassVariableBool(false)
	{}

	TestStructDefinedInSameFileAsClass::~TestStructDefinedInSameFileAsClass()
	{}

	void TestStructDefinedInSameFileAsClass::TestStructDefinedInSameFileAsClassFunction()
	{}

	TestClassDefinedInSameFileAsStruct::TestClassDefinedInSameFileAsStruct() : m_TestClassDefinedInSameFileAsStructVariableString(),
		m_TestClassDefinedInSameFileAsStructVariableFloat(0.0f),
		m_TestClassDefinedInSameFileAsStructVariableInt(0),
		m_TestClassDefinedInSameFileAsStructVariableBool(false)
	{}

	TestClassDefinedInSameFileAsStruct::~TestClassDefinedInSameFileAsStruct()
	{}

	void TestClassDefinedInSameFileAsStruct::TestClassDefinedInSameFileAsStructFunction()
	{}
}