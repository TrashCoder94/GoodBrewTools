#include "TestClassReflectionMultipleInOneFile.h"

namespace GB
{
	TestClassAReflection::TestClassAReflection() : m_TestClassAVariableString(),
		m_TestClassAVariableFloat(0.0f),
		m_TestClassAVariableInt(0),
		m_TestClassAVariableBool(false)
	{}

	TestClassAReflection::~TestClassAReflection()
	{}

	void TestClassAReflection::TestClassAFunction()
	{}

	TestClassBReflection::TestClassBReflection() : m_TestClassBVariableString(),
		m_TestClassBVariableFloat(0.0f),
		m_TestClassBVariableInt(0),
		m_TestClassBVariableBool(false)
	{}

	TestClassBReflection::~TestClassBReflection()
	{}

	void TestClassBReflection::TestClassBReflection()
	{}
}
