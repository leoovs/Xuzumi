/**
 * @file Xuzumi/Memory/Deleter.hpp
 * 
 * @brief Defines deleters for smart pointers.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	/**
	 * @brief Functor that implements default deleter.
	 * 
	 * @tparam T Type of the resource.
	 */
	template<typename T>
	class DefaultDeleter
	{
	public:
		/**
		 * @typedef PointerType
		 * 
		 * @brief Type alias for a pointer to a resource.
		 */
		using PointerType = std::add_pointer_t<std::remove_extent_t<T>>;

		/**
		 * @brief Deletes a resource via the passed pointer.
		 *
		 * @param ptr The pointer to a resource.
		 */
		void operator()(PointerType ptr)
		{
			if constexpr (std::is_array_v<T>)
			{
				delete[] ptr;
				return;
			}
			delete ptr;
		}
	};
}
