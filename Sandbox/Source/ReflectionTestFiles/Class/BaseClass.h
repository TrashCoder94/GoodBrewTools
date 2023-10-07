#pragma once

#include <GBReflect.h>

namespace GB
{
	class BaseClass
	{
	public:
		virtual ~BaseClass() = default;
		virtual const reflect::TypeDescriptor_Struct& GetTypeDescription() const = 0;
	};
}