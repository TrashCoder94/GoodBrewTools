#pragma once

#include <GBReflect.h>

namespace GB
{
	struct BaseStruct
	{
	public:
		virtual ~BaseStruct() = default;
		virtual const reflect::TypeDescriptor_Struct& GetTypeDescription() const = 0;
	};
}