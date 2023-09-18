/**
 * @file Xuzumi/Core/Templates/IsCompatible.hpp
 * 
 * @brief Defines a type trait to determine types compatibility.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi::Internal
{
	template<typename FirstT, typename SecondT>
	struct IsCompatibleDetails
	{
		using SecondType = std::remove_extent_t<SecondT>;

		inline static constexpr bool IsConvertible =
			std::is_convertible_v<FirstT*, SecondT*>;

		inline static constexpr bool SameType = std::is_same_v<FirstT, SecondType>;
	};
}

namespace Xuzumi
{
	/**
	 * Type trait to check whether two types are compatible.
	 * 
	 * This type trait implements the C++17 design of type compatibility,
	 * see https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr.
	 * 
	 * @tparam FirstT Type that has to be compatible with `SecondT`.
	 * @tparam SecondT Type that `FirstT` has to be compatible with.
	 */
	template<typename FirstT, typename SecondT>
	struct IsCompatible
	{
	private:
		using Details = Internal::IsCompatibleDetails<FirstT, SecondT>;

	public:
		/**
		 * Holds a boolean value indicating whether the specified types are
		 * compatible.
		 */
		inline static constexpr bool Value = Details::IsConvertible ||
			(std::is_array_v<SecondT> && Details::SameType);
	};

	/**
	 * Holds a boolean value indicating whether `FirstT` is compatible with
	 * `SecondT`.
	 * 
	 * The value is determined by Xuzumi::IsCompatible<FirstT, SecondT>::Value.
	 * 
	 * @see Xuzumi::IsCompatible<FirstT, SecondT>::Value
	 * @see Xuzumi::IsCompatible
	 */
	template<typename FirstT, typename SecondT>
	inline constexpr bool IsCompatibleV =
		IsCompatible<FirstT, SecondT>::Value;
}
