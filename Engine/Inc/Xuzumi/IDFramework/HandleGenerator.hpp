/**
 * @file Xuzumi/IDFramework/HandleGenerator.hpp
 * 
 * @brief Defines utilities for managing integer handles that are part of
 * engine's ID framework.
 */

#pragma once

#include "Xuzumi/IDFramework/IDBase.hpp"

namespace Xuzumi::Internal
{
	/**
	 * @typedef HandleID
	 * 
	 * @brief Common type used for storing integer handle IDs.
	 */
	using HandleID = IDType;

	/**
	 * @brief Invalid integer handle.
	 * 
	 * Handles with this value cannot exist in the generator registry and
	 * considered invalid.
	 */
	inline constexpr auto kInvalidHandleID = HandleID(-1);

	/**
	 * @brief Generates and registers integer handles.
	 * 
	 * Current implementation of this class is not thread-safe.
	 */
	class HandleGenerator
	{
	public:
		/**
		 * @brief Creates a unique handle ID.
		 * 
		 * Handle IDs are generated in ascending order starting from zero, which
		 * is why they are easy to use as an array indices. IDs that were previously
		 * destroyed will be reused, that allows to reuse array cells too. There is
		 * no need to check the returned value against `kInvalidHandleID`, as it is
		 * always valid.
		 * 
		 * @return An object of type `HandleID` containing value that is unique to
		 * the current `HandleGenerator` object.
		 */
		HandleID Create();

		/**
		 * @brief Invalidates the passed handle and makes it reusable.
		 * 
		 * Does nothing if the passed ID does not exist.
		 *
		 * @param handleID An object of type `HandleID` containing valid handle ID.
		 */
		void Destroy(HandleID handleID);

		/**
		 * @brief Checks whether the specified handle ID exists in the registry.
		 * 
		 * @param handleID An object of type `HandleID` containing handle ID. 
		 * 
		 * @retval true Specified ID exists and is valid.
		 * @retval false Specified ID does not exist and is invalid.
		 */
		bool Exists(HandleID handleID) const;

		/**
		 * @brief Invalidates all previously created IDs and makes registry forget
		 * recycled IDs.
		 */
		void Reset();

	private:
		std::unordered_set<HandleID> mAliveHandleIDs;
		std::queue<HandleID> mRecycledIDs;
		HandleID mCurrentHandleID = 0;
	};
}
