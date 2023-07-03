/**
 * @file Xuzumi/IDFramework/IDBase.hpp
 * 
 * @brief Defines common types for working with IDs
 * of any kind.
 */

#ifndef XUZUMI_IDFRAMEWORK_IDBASE_HPP_
#define XUZUMI_IDFRAMEWORK_IDBASE_HPP_

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	/**
	 * @typedef IDType
	 * 
	 * @brief Common type used to represent ID of any kind.
	 */
	using IDType = std::uint64_t;

	inline constexpr IDType kInvalidID = IDType(-1);
}

#endif // XUZUMI_IDFRAMEWORK_IDBASE_HPP_
