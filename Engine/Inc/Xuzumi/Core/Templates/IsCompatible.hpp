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
	template<typename FirstT, typename SecondT>
	struct IsCompatible
	{
		using Details = Internal::IsCompatibleDetails<FirstT, SecondT>;

		inline static constexpr bool Value = Details::IsConvertible ||
			(std::is_array_v<SecondT> && Details::SameType);
	};

	template<typename FirstT, typename SecondT>
	inline constexpr bool IsCompatibleV =
		IsCompatible<FirstT, SecondT>::Value;
}
