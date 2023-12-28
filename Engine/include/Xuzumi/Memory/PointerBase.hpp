#pragma once

namespace Xuzumi::Internal
{
	template<typename T>
	struct PointerTraits
	{
		static_assert(
			!std::is_reference_v<T>,
			"Xuzumi: pointer to a reference is not allowed"
		);
	
		using ValueType = std::remove_extent_t<T>;
		using PointerType = std::add_pointer_t<ValueType>;
		using ReferenceType = std::add_lvalue_reference_t<ValueType>;
	
		template<typename OtherT, bool IsArrayT>
		struct OtherTypeImpl {};
	
		template<typename OtherT>
		struct OtherTypeImpl<OtherT, true>
		{
			using Type = OtherT[];
		};
	
		template<typename OtherT>
		struct OtherTypeImpl<OtherT, false>
		{
			using Type = OtherT;
		};

		template<typename OtherT>
		using OtherType =
			typename OtherTypeImpl<OtherT, std::is_array_v<T>>::Type;
	};
}
