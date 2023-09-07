/**
 * @file Xuzumi/IDFramework/IDBase.hpp
 *
 * @brief Defines core ID framework infrastructure.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	/**
	 * @typedef IDType
	 * 
	 * @brief Common type that is used as base type for any other derived ID
	 * types.
	 */
	using IDType = std::uint64_t;

	/**
	 * @brief Invalid ID value.
	 */
	inline constexpr auto kInvalidID = IDType(-1);
}
