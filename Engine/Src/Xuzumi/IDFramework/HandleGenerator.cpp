#include "Xuzumi/IDFramework/HandleGenerator.hpp"

namespace Xuzumi::Internal
{
	HandleID HandleGenerator::Create()
	{
		if (mRecycledIDs.size())
		{
			HandleID id = mRecycledIDs.front();
			mRecycledIDs.pop();

			mAliveHandleIDs.insert(id);
			return id;
		}
	
		mAliveHandleIDs.insert(mCurrentHandleID);

		return mCurrentHandleID++;
	}

	void HandleGenerator::Destroy(HandleID handleID)
	{
		if (!Exists(handleID))
		{
			return;
		}

		mAliveHandleIDs.erase(handleID);
		mRecycledIDs.push(handleID);
	}

	bool HandleGenerator::Exists(HandleID handleID) const
	{
		return handleID < mCurrentHandleID
			&& mAliveHandleIDs.find(handleID) != mAliveHandleIDs.end();
	}

	void HandleGenerator::Reset()
	{
		mCurrentHandleID = 0;
		mAliveHandleIDs.clear();
		
		std::queue<HandleID> empty;
		mRecycledIDs.swap(empty);
	}
}
