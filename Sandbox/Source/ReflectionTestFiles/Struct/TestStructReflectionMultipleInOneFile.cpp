#include "TestStructReflectionMultipleInOneFile.h"

namespace GB
{
	TestStructAReflection::TestStructAReflection() : m_TestStructAVariableString(),
		m_TestStructAVariableFloat(0.0f),
		m_TestStructAVariableInt(0),
		m_TestStructAVariableBool(false)
	{}

	TestStructAReflection::~TestStructAReflection()
	{}

	void TestStructBReflection::TestStructBReflection()
	{}

	TestStructBReflection::TestStructBReflection() : m_TestStructBVariableString(),
		m_TestStructBVariableFloat(0.0f),
		m_TestStructBVariableInt(0),
		m_TestStructBVariableBool(false)
	{}

	TestStructBReflection::~TestStructBReflection()
	{}

	void TestStructBReflection::TestStructBFunction()
	{}
}