#pragma once

#include "GBClassRegistry.h"
#include "GBVariableFlags.h"
#include "ReflectionMacros.h"

// Label variables inside classes with this macro (and either Edit or Hidden)
#define GBVARIABLE(...)

// Label functions inside classes with this macro
#define GBFUNCTION(...)

// Include this macro in your class to setup the relevant reflection functions
#define GB_REFLECT() \
	REFLECT() \
	friend class GB::ClassRegistry; \
	public: virtual const reflect::TypeDescriptor_Struct& GetTypeDescription() const override;

#define GB_REFLECT_CLASS_BEGIN(type) \
    reflect::TypeDescriptor_Struct type::Reflection{type::initReflection}; \
    void type::initReflection(reflect::TypeDescriptor_Struct* typeDesc) { \
        using T = type; \
		GB::ClassRegistry::Get().Register<type>(#type);\
        typeDesc->name = #type; \
        typeDesc->size = sizeof(T); \
        typeDesc->members = {

#define GB_REFLECT_CLASS_MEMBER(name) \
            REFLECT_STRUCT_MEMBER(name)

#define GB_REFLECT_CLASS_END() \
        REFLECT_STRUCT_END()

#define GB_REFLECT_BEGIN(type) \
	const reflect::TypeDescriptor_Struct& type::GetTypeDescription() const\
	{\
		reflect::TypeDescriptor* typeDesc = reflect::TypeResolver<type>::get();\
		reflect::TypeDescriptor_Struct* structDesc = static_cast<reflect::TypeDescriptor_Struct*>(typeDesc);\
		return *structDesc;\
	}\
	GB_REFLECT_CLASS_BEGIN(type)
#define GB_REFLECT_MEMBER(name) GB_REFLECT_CLASS_MEMBER(name)
#define GB_REFLECT_FUNCTION(name)
#define GB_REFLECT_END() GB_REFLECT_CLASS_END()

namespace GB
{
	static const std::string MACRO_REFLECT = REFLECTION_MACRO_STRINGIFY(GB_REFLECT);
	static const std::string MACRO_VARIABLE = REFLECTION_MACRO_STRINGIFY(GBVARIABLE);
	static const std::string MACRO_FUNCTION = REFLECTION_MACRO_STRINGIFY(GBFUNCTION);
	static const std::string MACRO_VARIABLE_FLAG_EDIT = REFLECTION_MACRO_STRINGIFY(Edit);
	static const std::string MACRO_VARIABLE_FLAG_HIDDEN = REFLECTION_MACRO_STRINGIFY(Hidden);

	static const std::string AUTO_GENERATED_FILE_SUFFIX = "_gen.cpp";
}
