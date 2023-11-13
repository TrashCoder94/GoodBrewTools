#pragma once

namespace reflect
{
	enum class FieldType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool,
		String,
		Colour,
		Texture,
		Class, // Anything inheriting from IReflectableClass or IReflectableStruct
		ClassPtr,
		Vector,
		UniquePtr,
		SharedPtr,
		WeakPtr
	};
}