#pragma once

#include <GBReflect.h>

namespace GB
{
	struct BaseStructDefinedInSameFileAsBaseClass
	{
	public:
		virtual ~BaseStructDefinedInSameFileAsBaseClass() = default;
		virtual const reflect::TypeDescriptor_Struct& GetTypeDescription() const = 0;
	};

	class BaseClassDefinedInSameFileAsBaseStruct
	{
	public:
		virtual ~BaseClassDefinedInSameFileAsBaseStruct() = default;
		virtual const reflect::TypeDescriptor_Struct& GetTypeDescription() const = 0;
	};
}