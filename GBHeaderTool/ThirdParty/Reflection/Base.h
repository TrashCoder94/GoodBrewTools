#pragma once

#include <memory>
#include <type_traits>

namespace reflect
{
	template<class T>
	struct is_unique_ptr : std::false_type {};

	template<class T>
	struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};

	template<class T>
	struct is_shared_ptr : std::false_type {};

	template<class T>
	struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

	template<class T>
	struct is_weak_ptr : std::false_type {};

	template<class T>
	struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {};
}
