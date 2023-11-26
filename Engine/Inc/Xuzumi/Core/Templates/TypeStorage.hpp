#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	template<typename T>
	struct TypeStorage
	{
		alignas(T) std::array<std::byte, sizeof(T)> Memory;

		template<typename... ArgsT>
		T* Construct(ArgsT&&... args)
		{
			return new (Memory.data()) T(std::forward<ArgsT>(args)...);
		}
	
		void Destruct()
		{
			reinterpret_cast<T*>(Memory.data())->~T();
		}
	};
}
