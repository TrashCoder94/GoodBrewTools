#pragma once

#include <string>
#include "FieldType.h"

namespace reflect
{
	//--------------------------------------------------------
	// Base class of all type descriptors
	//--------------------------------------------------------

	struct TypeDescriptor {
		const char* name;
		size_t size;
		FieldType fieldType;

		TypeDescriptor(const char* name, size_t size, FieldType type) : name{ name }, size{ size }, fieldType{ type } {}
		virtual ~TypeDescriptor() {}
		virtual std::string getFullName() const { return name; }
		virtual FieldType getFieldType() const { return fieldType; }
		virtual std::string getFieldTypeAsString() const
		{
			std::string fieldTypeString{ "" };
			switch (fieldType)
			{
				case FieldType::None:					fieldTypeString = "None";					break;
				case FieldType::Float:					fieldTypeString = "Float";					break;
				case FieldType::Float2:					fieldTypeString = "Float2";					break;
				case FieldType::Float3:					fieldTypeString = "Float3";					break;
				case FieldType::Float4:					fieldTypeString = "Float4";					break;
				case FieldType::Mat3:					fieldTypeString = "Mat3";					break;
				case FieldType::Mat4:					fieldTypeString = "Mat4";					break;
				case FieldType::Int:					fieldTypeString = "Int";					break;
				case FieldType::Int2:					fieldTypeString = "Int2";					break;
				case FieldType::Int3:					fieldTypeString = "Int3";					break;
				case FieldType::Int4:					fieldTypeString = "Int4";					break;
				case FieldType::Bool:					fieldTypeString = "Bool";					break;
				case FieldType::String:					fieldTypeString = "String";					break;
				case FieldType::Colour:					fieldTypeString = "Colour";					break;
				case FieldType::Texture:				fieldTypeString = "Texture";				break;
				case FieldType::Class:					fieldTypeString = "Class";					break;
				case FieldType::Vector:					fieldTypeString = "Vector";					break;
				default: break;
			}

			return fieldTypeString;
		}
		virtual void dump(const void* obj, int indentLevel = 0) const = 0;
	};


	//--------------------------------------------------------
	// Finding type descriptors
	//--------------------------------------------------------

	// Declare the function template that handles primitive types such as int, std::string, etc.:
	template <typename T>
	TypeDescriptor* getPrimitiveDescriptor();

	// A helper class to find TypeDescriptors in different ways:
	struct DefaultResolver {
		template <typename T> static char func(decltype(&T::Reflection));
		template <typename T> static int func(...);
		template <typename T>
		struct IsReflected {
			enum { value = (sizeof(func<T>(nullptr)) == sizeof(char)) };
		};

		// This version is called if T has a static member named "Reflection":
		template <typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
		static TypeDescriptor* get() {
			return &T::Reflection;
		}

		// This version is called otherwise:
		template <typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
		static TypeDescriptor* get() {
			return getPrimitiveDescriptor<T>();
		}
	};

	// This is the primary class template for finding all TypeDescriptors:
	template <typename T>
	struct TypeResolver {
		static TypeDescriptor* get() {
			return DefaultResolver::get<T>();
		}
	};
}