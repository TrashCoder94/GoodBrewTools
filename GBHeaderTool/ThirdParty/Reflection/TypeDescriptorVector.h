#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "TypeDescriptor.h"

namespace reflect
{

	//--------------------------------------------------------
	// Type descriptors for std::vector
	//--------------------------------------------------------

	struct TypeDescriptor_StdVector : TypeDescriptor {
		TypeDescriptor* itemType;
		void(*addNewItem)(const void*);
		void(*removeItem)(const void*, size_t);
		void(*clearAllItems)(const void*);
		size_t(*getSize)(const void*);
		const void* (*getItem)(const void*, size_t);

		template <typename ItemType>
		TypeDescriptor_StdVector(ItemType*, FieldType vectorFieldType = FieldType::Vector)
			: TypeDescriptor{ "std::vector<>", sizeof(std::vector<ItemType>), vectorFieldType },
			itemType{ TypeResolver<ItemType>::get() } {
			addNewItem = [](const void* vecPtr) {
				auto& vec = *(std::vector<ItemType>*) vecPtr;
				vec.emplace_back();
				};
			removeItem = [](const void* vecPtr, size_t index) {
				auto& vec = *(std::vector<ItemType>*) vecPtr;
				vec.erase(vec.begin() + index);
				};
			clearAllItems = [](const void* vecPtr) {
				auto& vec = *(std::vector<ItemType>*) vecPtr;
				vec.clear();
				};
			getSize = [](const void* vecPtr) -> size_t {
				const auto& vec = *(const std::vector<ItemType>*) vecPtr;
				return vec.size();
				};
			getItem = [](const void* vecPtr, size_t index) -> const void* {
				const auto& vec = *(const std::vector<ItemType>*) vecPtr;
				return &vec[index];
				};
		}
		virtual std::string getFullName() const override {
			return std::string("std::vector<") + itemType->getFullName() + ">";
		}
		virtual void dump(const void* obj, int indentLevel) const override {
			size_t numItems = getSize(obj);
			std::cout << getFullName();
			if (numItems == 0) {
				std::cout << "{}";
			}
			else {
				std::cout << "{" << std::endl;
				for (size_t index = 0; index < numItems; index++) {
					std::cout << std::string(4 * (indentLevel + 1), ' ') << "[" << index << "] ";
					itemType->dump(getItem(obj, index), indentLevel + 1);
					std::cout << std::endl;
				}
				std::cout << std::string(4 * indentLevel, ' ') << "}";
			}
		}
	};

	// Partially specialize TypeResolver<> for std::vectors:
	template <typename T>
	struct TypeResolver<std::vector<T>> {
	public:
		static TypeDescriptor* get() {
			static TypeDescriptor_StdVector typeDesc{ (T*) nullptr };
			return &typeDesc;
		}
	};
}
