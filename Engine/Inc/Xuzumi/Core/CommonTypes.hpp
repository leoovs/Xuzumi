/**
 * @file Xuzumi/Core/CommonTypes.hpp
 * 
 * @brief Defines common type aliases used in the Xuzumi engine.
 */

#ifndef XUZUMI_CORE_COMMON_TYPES_HPP_
#define XUZUMI_CORE_COMMON_TYPES_HPP_

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	/**
	 * @brief Alias for byte.
	 */
	using Byte = unsigned char;

	/**
	 * @brief Alias for atomic byte.
	 */
	using AtomicByte = std::atomic<Byte>;

	/**
	 * @brief Alias for signed integer that is at least 8-bit long.
	 */
	using Int8 = signed char;

	/**
	 * @brief Alias for atomic Int8.
	 */
	using AtomicInt8 = std::atomic<Int8>;

	/**
	 * @brief Alias for unsigned integer that is at least 8-bit long.
	 */
	using UInt8 = unsigned char;

	/**
	 * @brief Alias for atomic UInt8.
	 */
	using AtomicUInt8 = std::atomic<UInt8>;

	/**
	 * @brief Alias for signed integer that is at least 16-bit long.
	 */
	using Int16 = signed short;

	/**
	 * @brief Alias for atomic Int16.
	 */
	using AtomicInt16 = std::atomic<Int16>;
	
	/**
	 * @brief Alias for unsigned integer that is at least 16-bit long.
	 */
	using UInt16 = unsigned short;

	/**
	 * @brief Alias for atomic UInt16.
	 */
	using AtomicUInt16 = std::atomic<UInt16>;

	/**
	 * @brief Alias for signed integer that is at least 32-bit long.
	 */
	using Int32 = signed long;

	/**
	 * @brief Alias for atomic Int32.
	 */
	using AtomicInt32 = std::atomic<Int32>;

	/**
	 * @brief Alias for unsigned integer that is at least 32-bit long.
	 */
	using UInt32 = unsigned long;

	/**
	 * @brief Alias for atomic UInt32.
	 */
	using AtomicUInt32 = std::atomic<UInt32>;

	/**
	 * @brief Alias for signed integer that is at least 64-bit long.
	 */
	using Int64 = signed long long int;

	/**
	 * @brief Alias for atomic Int64.
	 */
	using AtomicInt64 = std::atomic<Int64>;

	/**
	 * @brief Alias for unsigned integer that is at least 64-bit long.
	 */
	using UInt64 = unsigned long long int;

	/**
	 * @brief Alias for atomic UInt64.
	 */
	using AtomicUInt64 = std::atomic<UInt64>;

	/**
	 * @brief Alias for the size type.
	 *
	 * The size type represents the size of an object or memory in bytes.
	 * It is platform-dependent and is typically typedef'd as std::size_t.
	 */
	using SizeType = std::size_t;

	/**
	 * @brief Alias for atomic SizeType.
	 */
	using AtomicSizeType = std::atomic_size_t;

	/**
	 * @brief Alias for the pointer difference type.
	 *
	 * The pointer difference type represents the difference between two pointers.
	 * It is platform-dependent and is typically typedef'd as std::ptrdiff_t.
	 */
	using PtrDiff = std::ptrdiff_t;

	/**
	 * @brief Alias for atomic PtrDiff.
	 */
	using AtomicPtrDiff = std::atomic_ptrdiff_t;
}

#endif // XUZUMI_CORE_COMMON_TYPES_HPP_
