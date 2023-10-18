#pragma once

#include "Xuzumi/IDFramework/IDBase.hpp"

namespace Xuzumi
{
	/**
	 * @typedef TypeID
	 * 
	 * @brief Common type used for storing a type identifier.
	 */
	using TypeID = IDType;

	/**
	 * @brief Represents invalid type identifier.
	 */
	inline constexpr auto kInvalidTypeID = TypeID(kInvalidID); 

	namespace Internal
	{
		TypeID GenerateNextTypeID();
	}

	/**
	 * @brief Retrieves the type identifier of the @p ClassT.
	 * 
	 * @tparam ClassT The type which type identifier is required.
	 *
	 * @return A `TypeID` instance uniquely identifying the @p ClassT.
	 */
	template<typename ClassT>
	TypeID GetTypeID()
	{
		static TypeID sClassTypeID = Internal::GenerateNextTypeID();
		return sClassTypeID;
	}

	/**
	 * @brief Retrieves the type identifier of the @p classInstance type.
	 * 
	 * @tparam ClassT The type of the @p classInstance.
	 *
	 * @param classInstance A reference to an object of the @p ClassT type.
	 *
	 * @return A `TypeID` instance uniquely identifying the @p ClassT.
	 */
	template<typename ClassT>
	TypeID GetTypeID(const ClassT& classInstance)
	{
		return GetTypeID<ClassT>();
	}
}
