/**
 * @file Xuzumi/Core/CommonTypes.hpp
 * 
 * @brief Defines common type aliases used in the Xuzumi engine.
 */

#ifndef XUZUMI_CORE_COMMON_TYPES_HPP
#define XUZUMI_CORE_COMMON_TYPES_HPP

#include <cstddef>

namespace Xuzumi
{
	/**
	 * @brief Alias for byte.
	 */
	using Byte = unsigned char;

	/**
	 * @brief Alias for signed integer that is at least 8-bit long.
	 */
	using Int8 = signed char;

	/**
	 * @brief Alias for unsigned integer that is at least 8-bit long.
	 */
	using UInt8 = unsigned char;

	/**
	 * @brief Alias for signed integer that is at least 16-bit long.
	 */
	using Int16 = signed short;
	
	/**
	 * @brief Alias for unsigned integer that is at least 16-bit long.
	 */
	using UInt16 = unsigned short;

	/**
	 * @brief Alias for signed integer that is at least 32-bit long.
	 */
	using Int32 = signed long;

	/**
	 * @brief Alias for unsigned integer that is at least 32-bit long.
	 */
	using UInt32 = unsigned long;

	/**
	 * @brief Alias for signed integer that is at least 64-bit long.
	 */
	using Int64 = signed long long int;

	/**
	 * @brief Alias for unsigned integer that is at least 64-bit long.
	 */
	using UInt64 = unsigned long long int;

	/**
	 * @brief Alias for the size type.
	 *
	 * The size type represents the size of an object or memory in bytes.
	 * It is platform-dependent and is typically typedef'd as std::size_t.
	 */
	using SizeType = std::size_t;

	/**
	 * @brief Alias for the pointer difference type.
	 *
	 * The pointer difference type represents the difference between two pointers.
	 * It is platform-dependent and is typically typedef'd as std::ptrdiff_t.
	 */
	using PtrDiff = std::ptrdiff_t;
}

#endif // XUZUMI_CORE_COMMON_TYPES_HPP
