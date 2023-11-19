#pragma once

#include "IReflectable.h"
#include "GBReflectionMacros.h"

namespace GB
{
	/**
	 * Note: This has been setup to work around the vector of bools issue.
	 * Setup a vector of this struct and reflect this instead of just a regular vector of bools.
	 * Otherwise it won't work!
	 */
	struct GBBool : public reflect::IReflectableStruct
	{
		GB_REFLECT()

	public:
		GBBool() = default;
		~GBBool() = default;

		void SetBool(const bool newValue)	{ m_Bool = newValue; }
		const bool GetBool()				{ return m_Bool; }
		const bool GetBool() const			{ return m_Bool; }

		explicit operator bool()			{ return m_Bool; }

	private:
		GBVARIABLE(Edit)
		bool m_Bool = false;
	};
}
