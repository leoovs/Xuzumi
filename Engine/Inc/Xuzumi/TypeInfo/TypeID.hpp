/**
 * @file Xuzumi/TypeInfo/TypeID.hpp
 * 
 * @brief Defines common types and functions to
 * work with type identification.
 */

#ifndef XUZUMI_TYPEINFO_TYPEID_HPP_
#define XUZUMI_TYPEINFO_TYPEID_HPP_

#include <Xuzumi/IDFramework/IDBase.hpp>

namespace Xuzumi
{
	/**
	 * @typedef TypeID
	 * 
	 * @brief Common type used to represent type ID.
	 */
	using TypeID = IDType;

	inline constexpr TypeID kInvalidTypeID = TypeID(-1); 

	namespace Internal
	{
		TypeID GenerateNextTypeID();
	}

	/**
	 * @breif Retrieves ID of the specified type.
	 * 
	 * @tparam ClassT A class to query its type ID.
	 */
	template<typename ClassT>
	TypeID GetTypeID()
	{
		static TypeID sClassTypeID = Internal::GenerateNextTypeID();
		return sClassTypeID;
	}

	/**
	 * @brief Convenience function that retrieves type ID of an object.
	 * 
	 * @tparam ClassT A class to query its ID. Supposed to be implicit.
	 * 
	 * @param classInstance A class instance to take its type ID. 
	 */
	template<typename ClassT>
	TypeID GetTypeID(const ClassT& classInstance)
	{
		return GetTypeID<ClassT>();
	}
}

#endif // XUZUMI_TYPEINFO_TYPEID_HPP_
