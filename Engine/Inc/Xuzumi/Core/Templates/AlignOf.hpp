#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	/**
	 * @brief Type trait to retrieve the alignment of the @p ClassT type.
	 *
	 * @tparam ClassT The type which alignment is to be retrieved.
	 */
	template<typename ClassT>
	struct AlignOf
	{
		/**
		 * @brief Holds the alignment of the @p ClassT type.
		 */
		inline constexpr static std::size_t Value = alignof(ClassT);
	};

	/**
	 * @brief Specialization of the `Xuzumi::AlignOf` type trait for the `void`
	 * type.
	 */
	template<>
	struct AlignOf<void>
	{
		/**
		 * @brief The alignment of `void` is considered to be one.
		 */
		inline constexpr static std::size_t Value = 1;
	};

	/**
	 * @brief Holds the alignment of the @p ClassT type.
	 *
	 * The value is determined by `Xuzumi::AlignOf<ClassT>::Value`.
	 *
	 * @tparam ClassT The type name which alignment is to be stored.
	 *
	 * @see Xuzumi::AlignOf<ClassT>::Value
	 * @see Xuzumi::AlignOf
	 */
	template<typename ClassT>
	inline constexpr std::size_t AlignOfV = AlignOf<ClassT>::Value;
}
