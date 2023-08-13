#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	template<typename T>
	class DefaultDeleter
	{
	public:
		using ValueType = std::remove_extent_t<T>;
		using PointerType = std::add_pointer_t<ValueType>;

		void operator()(PointerType resourcePointer)
		{
			if constexpr (std::is_array_v<T>)
			{
				delete[] resourcePointer;
				return;
			}
			else
			{
				delete resourcePointer;
			}
		}
	};
}
