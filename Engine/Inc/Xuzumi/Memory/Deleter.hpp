#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	template<typename T>
	class DefaultDeleter
	{
	public:
		using PointerType = std::add_pointer_t<std::remove_extent_t<T>>;

		void operator()(PointerType ptr)
		{
			if constexpr (std::is_array_v<T>)
			{
				delete[] ptr;
				return;
			}
			delete ptr;
		}
	};
}
