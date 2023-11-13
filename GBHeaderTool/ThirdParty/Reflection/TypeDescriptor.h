#pragma once

#include <memory>
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
				case FieldType::ClassPtr:				fieldTypeString = "ClassPtr";				break;
				case FieldType::Vector:					fieldTypeString = "Vector";					break;
				case FieldType::UniquePtr:				fieldTypeString = "UniquePtr";				break;
				case FieldType::SharedPtr:				fieldTypeString = "SharedPtr";				break;
				case FieldType::WeakPtr:				fieldTypeString = "WeakPtr";				break;
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

	//--------------------------------------------------------
	// Type descriptor for UniquePtr
	//--------------------------------------------------------
	struct TypeDescriptor_StdUniquePtr : TypeDescriptor {
		TypeDescriptor* targetType;
		const void* (*getTargetConst)(const void*);
		void* (*getTarget)(const void*);

		// Template constructor:
		template <typename TargetType>
		TypeDescriptor_StdUniquePtr(TargetType* /* dummy argument */)
			: TypeDescriptor{ "std::unique_ptr<>", sizeof(std::unique_ptr<TargetType>), FieldType::UniquePtr },
			targetType{ TypeResolver<TargetType>::get() } {
			getTargetConst = [](const void* uniquePtrPtr) -> const void* {
				const auto& uniquePtr = *(const std::unique_ptr<TargetType>*) uniquePtrPtr;
				return uniquePtr.get();
				};
			getTarget = [](const void* uniquePtrPtr) -> void* {
				const auto& uniquePtr = *(const std::unique_ptr<TargetType>*) uniquePtrPtr;
				return uniquePtr.get();
				};
		}
		virtual std::string getFullName() const override {
			return std::string("std::unique_ptr<") + targetType->getFullName() + ">";
		}
		virtual void dump(const void* obj, int indentLevel) const override {
			std::cout << getFullName() << "{";
			const void* targetObj = getTarget(obj);
			if (targetObj == nullptr) {
				std::cout << "nullptr";
			}
			else {
				std::cout << std::endl;
				std::cout << std::string(4 * (indentLevel + 1), ' ');
				targetType->dump(targetObj, indentLevel + 1);
				std::cout << std::endl;
				std::cout << std::string(4 * indentLevel, ' ');
			}
			std::cout << "}";
		}
	};

	// Partially specialize TypeResolver<> for std::unique_ptr<>:
	template <typename T>
	struct TypeResolver<std::unique_ptr<T>> {
	public:
		static TypeDescriptor* get() {
			static TypeDescriptor_StdUniquePtr typeDesc{ (T*) nullptr };
			return &typeDesc;
		}
	};

	//--------------------------------------------------------
	// Type descriptor for SharedPtr
	//--------------------------------------------------------
	struct TypeDescriptor_StdSharedPtr : TypeDescriptor {
		TypeDescriptor* targetType;
		const void* (*getTargetConst)(const void*);
		void* (*getTarget)(const void*);

		// Template constructor:
		template <typename TargetType>
		TypeDescriptor_StdSharedPtr(TargetType* /* dummy argument */)
			: TypeDescriptor{ "std::shared_ptr<>", sizeof(std::shared_ptr<TargetType>), FieldType::SharedPtr },
			targetType{ TypeResolver<TargetType>::get() } {
			getTargetConst = [](const void* sharedPtrPtr) -> const void* {
				const auto& sharedPtr = *(const std::shared_ptr<TargetType>*) sharedPtrPtr;
				return sharedPtr.get();
				};
			getTarget = [](const void* sharedPtrPtr) -> void* {
				const auto& sharedPtr = *(const std::shared_ptr<TargetType>*) sharedPtrPtr;
				return sharedPtr.get();
				};
		}
		virtual std::string getFullName() const override {
			return std::string("std::shared_ptr<") + targetType->getFullName() + ">";
		}
		virtual void dump(const void* obj, int indentLevel) const override {
			std::cout << getFullName() << "{";
			const void* targetObj = getTarget(obj);
			if (targetObj == nullptr) {
				std::cout << "nullptr";
			}
			else {
				std::cout << std::endl;
				std::cout << std::string(4 * (indentLevel + 1), ' ');
				targetType->dump(targetObj, indentLevel + 1);
				std::cout << std::endl;
				std::cout << std::string(4 * indentLevel, ' ');
			}
			std::cout << "}";
		}
	};

	// Partially specialize TypeResolver<> for std::shared_ptr<>:
	template <typename T>
	struct TypeResolver<std::shared_ptr<T>> {
	public:
		static TypeDescriptor* get() {
			static TypeDescriptor_StdSharedPtr typeDesc{ (T*) nullptr };
			return &typeDesc;
		}
	};

	//--------------------------------------------------------
	// Type descriptor for WeakPtr
	//--------------------------------------------------------
	struct TypeDescriptor_StdWeakPtr : TypeDescriptor {
		TypeDescriptor* targetType;
		const void* (*getTargetConst)(const void*);
		void* (*getTarget)(const void*);

		// Template constructor:
		template <typename TargetType>
		TypeDescriptor_StdWeakPtr(TargetType* /* dummy argument */)
			: TypeDescriptor{ "std::weak_ptr<>", sizeof(std::weak_ptr<TargetType>), FieldType::WeakPtr },
			targetType{ TypeResolver<TargetType>::get() } {
			getTargetConst = [](const void* weakPtrPtr) -> const void* {
				const auto& weakPtr = *(const std::weak_ptr<TargetType>*) weakPtrPtr;
				if (std::shared_ptr<TargetType> pShared = weakPtr.lock())
					return pShared.get();
				return nullptr;
				};
			getTarget = [](const void* weakPtrPtr) -> void* {
				const auto& weakPtr = *(const std::weak_ptr<TargetType>*) weakPtrPtr;
				if (std::shared_ptr<TargetType> pShared = weakPtr.lock())
					return pShared.get();
				return nullptr;
				};
		}
		virtual std::string getFullName() const override {
			return std::string("std::weak_ptr<") + targetType->getFullName() + ">";
		}
		virtual void dump(const void* obj, int indentLevel) const override {
			std::cout << getFullName() << "{";
			const void* targetObj = getTarget(obj);
			if (targetObj == nullptr) {
				std::cout << "nullptr";
			}
			else {
				std::cout << std::endl;
				std::cout << std::string(4 * (indentLevel + 1), ' ');
				targetType->dump(targetObj, indentLevel + 1);
				std::cout << std::endl;
				std::cout << std::string(4 * indentLevel, ' ');
			}
			std::cout << "}";
		}
	};

	// Partially specialize TypeResolver<> for std::weak_ptr<>:
	template <typename T>
	struct TypeResolver<std::weak_ptr<T>> {
	public:
		static TypeDescriptor* get() {
			static TypeDescriptor_StdWeakPtr typeDesc{ (T*) nullptr };
			return &typeDesc;
		}
	};
}