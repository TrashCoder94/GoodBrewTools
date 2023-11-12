#pragma once

#include "TypeDescriptorStruct.h"

namespace reflect
{
	struct IReflectableStruct
	{
	public:
		IReflectableStruct() = default;
		virtual ~IReflectableStruct() = default;
		virtual const reflect::TypeDescriptor_Struct& GetTypeDescription() const = 0;
	};

	class IReflectableClass
	{
	public:
		IReflectableClass() = default;
		virtual ~IReflectableClass() = default;
		virtual const reflect::TypeDescriptor_Struct& GetTypeDescription() const = 0;
	};
}