#pragma once

#include "Xuzumi/IDFramework/IDBase.hpp"

namespace Xuzumi
{
	using TypeID = IDType;

	inline constexpr TypeID kInvalidTypeID = TypeID(kInvalidID); 

	namespace Internal
	{
		TypeID GenerateNextTypeID();
	}

	template<typename ClassT>
	TypeID GetTypeID()
	{
		static TypeID sClassTypeID = Internal::GenerateNextTypeID();
		return sClassTypeID;
	}

	template<typename ClassT>
	TypeID GetTypeID(const ClassT& classInstance)
	{
		return GetTypeID<ClassT>();
	}
}
