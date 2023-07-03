#ifndef XUZUMI_TYPEINFO_TYPEID_HPP_
#define XUZUMI_TYPEINFO_TYPEID_HPP_

#include "Xuzumi/IDFramework/IDBase.hpp"

namespace Xuzumi
{
	using TypeID = IDType;

	inline constexpr TypeID kInvalidTypeID = TypeID(-1); 

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

#endif // XUZUMI_TYPEINFO_TYPEID_HPP_
