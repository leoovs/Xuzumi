#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	/**
	 * @brief Type trait to retrieve the size of the @p ClassT type.
	 * 
	 * @tparam ClassT The type which size is to be retrieved.
	 */
	template<typename ClassT>
	struct SizeOf
	{
		/**
		 * @brief Holds the size of the @p ClassT type.
		 */
		inline static constexpr std::size_t Value = sizeof(ClassT);
	};

	/**
	 * @brief Specialization of the `Xuzumi::SizeOf` type trait for the `void`
	 * type.
	 *
	 * @see Xuzumi::SizeOf
	 */
	template<>
	struct SizeOf<void>
	{
		/**
		 * @brief The size of `void` is considered to be zero.
		 */
		inline static constexpr std::size_t Value = 0;
	};
	
	/**
	 * @brief Specialization of the `Xuzumi::SizeOf` type trait for the `T[]`
	 * type.
	 * 
	 * @see Xuzumi::SizeOf
	 */
	template<typename T>
	struct SizeOf<T[]>
	{
		/**
		 * @brief The size of `T[]` is considered to be the size of `T`.
		 */
		inline static constexpr std::size_t Value = sizeof(T);
	};

	/**
	 * @brief Holds the size of the @p ClassT type.
	 *
	 * The value is determined by `Xuzumi::SizeOf<ClassT>::Value`.
	 *
	 * @tparam ClassT The type name which size is to be stored.
	 *
	 * @see Xuzumi::SizeOf<ClassT>::Value
	 * @see Xuzumi::SizeOf
	 */
	template<typename ClassT>
	inline constexpr std::size_t SizeOfV = SizeOf<ClassT>::Value;
}
