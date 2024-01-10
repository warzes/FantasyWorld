#pragma once

#include "Object/Private/TypeTraits.h"

namespace se
{
	class ObjectSystem;

	template <typename T> 
	class Object
	{
	public:
		using TypeTraits = TypeTraits<T>;
		static_assert(TypeTraits::IsReference == 0, "Reference type cannot be used!");


	protected:

	};
} // namespace se