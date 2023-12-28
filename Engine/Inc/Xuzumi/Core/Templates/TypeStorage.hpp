#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	/**
	 * @brief Implements a utility for manual management of an object.
	 * 
	 * @tparam T The type of the stored object.
	 */
	template<typename T>
	struct TypeStorage
	{
		/**
		 * @brief The memory buffer that stored physical bytes of the object of the
		 * type @p T.
		 */
		alignas(T) std::array<std::byte, sizeof(T)> Memory;

		/**
		 * @brief Calls the constructor of @p T with the specified arguments.
		 * 
		 * @tparam ArgsT The types of the arguments passed to the constructor.
		 * 
		 * @param args The arguments passed to the constructor.
		 * 
		 * @return A type-casted pointer
		*/
		template<typename... ArgsT>
		T* Construct(ArgsT&&... args)
		{
			return new (Memory.data()) T(std::forward<ArgsT>(args)...);
		}
	
		/**
		 * @brief Calls the @p T destructor.
		 */
		void Destruct()
		{
			Cast()->~T();
		}

		/**
		 * @brief Interpret stored bytes as an object of type @p T.
		 *
		 * @return The pointer to the object of type @p T.
		 */
		T* Cast()
		{
			return reinterpret_cast<T*>(Memory.data());
		}
	};
}
