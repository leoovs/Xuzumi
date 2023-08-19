#pragma once

#include "Xuzumi/IDFramework/IDBase.hpp"

namespace Xuzumi::Internal
{
	using HandleID = IDType;

	inline constexpr HandleID kInvalidHandleID = HandleID(-1);

	class HandleGenerator
	{
	public:
		HandleID Create();
		void Destroy(HandleID handleID);
		bool Exists(HandleID handleID) const;

		void Reset();

	private:
		std::unordered_set<HandleID> mAliveHandleIDs;
		std::queue<HandleID> mRecycledIDs;
		HandleID mCurrentHandleID = 0;
	};
}
