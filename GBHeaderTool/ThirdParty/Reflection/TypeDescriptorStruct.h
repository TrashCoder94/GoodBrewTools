#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "TypeDescriptor.h"

namespace reflect
{
	//--------------------------------------------------------
	// Type descriptors for user-defined structs/classes
	//--------------------------------------------------------

	struct TypeDescriptor_Struct : TypeDescriptor {
		struct Member {
			const char* name;
			size_t offset;
			size_t size;
			TypeDescriptor* type;
		};

		std::vector<Member> members;

		TypeDescriptor_Struct() : TypeDescriptor{ nullptr, 0, FieldType::Class } {
		}
		TypeDescriptor_Struct(void (*init)(TypeDescriptor_Struct*), const FieldType inFieldType = FieldType::Class) : TypeDescriptor{ nullptr, 0, inFieldType } {
			init(this);
		}
		TypeDescriptor_Struct(const char* name, size_t size, const std::initializer_list<Member>& init) : TypeDescriptor{ nullptr, 0, FieldType::Class }, members{ init } {
		}
		virtual void dump(const void* obj, int indentLevel) const override {
			std::cout << name << " {" << std::endl;
			for (const Member& member : members) {
				std::cout << std::string(4 * (indentLevel + 1), ' ') << member.name << " = ";
				member.type->dump((char*)obj + member.offset, indentLevel + 1);
				std::cout << std::endl;
			}
			std::cout << std::string(4 * indentLevel, ' ') << "}";
		}
	};

	template <typename T>
	struct TypeResolver<T*> {
	public:
		static TypeDescriptor* get() {
			static TypeDescriptor_Struct typeDesc{ T::initReflection, FieldType::ClassPtr };
			return &typeDesc;
		}
	};
}