#include "Xuzumi/Memory/PoolAllocator.hpp"

namespace Xuzumi::Internal
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// MemoryBlock 
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	MemoryBlock::MemoryBlock(
		const PoolAllocatorSpecification& spec,
		TypeInfo resourceTypeInfo
	)
		: mSpec(spec)
		, mResourceTypeInfo(resourceTypeInfo)
	{
		mAvailableChunks.reserve(mSpec.BlockSize);
		AllocateMemory();
	}

	MemoryChunk* MemoryBlock::AcquireChunk()
	{
		if (mAvailableChunks.empty())
		{
			return nullptr;
		}

		MemoryChunk* chunk = mAvailableChunks.back();
		mAvailableChunks.pop_back();

		return chunk;
	}

	void MemoryBlock::DisposeChunk(MemoryChunk* chunk)
	{
		mAvailableChunks.push_back(chunk);
	}

	bool MemoryBlock::IsExhausted() const
	{
		return mAvailableChunks.empty();
	}

	void MemoryBlock::AllocateMemory()
	{
		std::size_t resourceSize = mResourceTypeInfo.Size;
		std::size_t headerSize = sizeof(MemoryChunk);
		std::size_t totalSize = resourceSize + headerSize;

		std::size_t resourceAlignment = mResourceTypeInfo.Alignment;
		std::size_t headerAlignment = alignof(MemoryChunk);
		std::size_t alignment = std::max(resourceAlignment, headerAlignment);

		std::size_t memorySize = mSpec.BlockSize * totalSize;

		mMemory = std::shared_ptr<void>(
			operator new(memorySize, std::align_val_t(alignment), std::nothrow),
			[alignment](void* memory)
			{
				operator delete(memory, std::align_val_t(alignment));
			}
		);

		// TODO: check if mMemory is nullptr and report error.

		auto bytes = reinterpret_cast<std::byte*>(mMemory.get());

		for (std::ptrdiff_t iChunk = 0; iChunk < mSpec.BlockSize; iChunk++)
		{
			std::byte* chunk = bytes + iChunk * totalSize;

			auto header = reinterpret_cast<MemoryChunk*>(chunk);
			header->ResourceMemory = header + 1;
			header->ResourceTypeID = mResourceTypeInfo.ID;
			mAvailableChunks.push_back(header);
		}
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// MemoryPool
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	MemoryPool::MemoryPool(
		const PoolAllocatorSpecification& spec,
		TypeInfo resourceTypeInfo
	)
		: mSpec(spec)
		, mResourceTypeInfo(resourceTypeInfo)
	{
	}

	MemoryChunk* MemoryPool::AcquireChunk()
	{
		MemoryBlock* activeBlock = GetActiveBlock();
		std::size_t blockIndex = activeBlock - &mBlocks.front();
		
		MemoryChunk* chunk = activeBlock->AcquireChunk();
		chunk->ParentBlockIndex = blockIndex;

		return chunk;
	}

	void MemoryPool::DisposeChunk(MemoryChunk* chunk)
	{
		std::size_t blockIndex = chunk->ParentBlockIndex;

		if (blockIndex >= mBlocks.size())
		{
			// TODO: report error.
			return;
		}

		MemoryBlock& block = mBlocks[blockIndex];
		block.DisposeChunk(chunk);

		mReplenishedBlocks.push_back(&block);
	}

	MemoryBlock* MemoryPool::GetActiveBlock()
	{
		while (!mReplenishedBlocks.empty()
			&& mReplenishedBlocks.front()->IsExhausted())
		{
			mReplenishedBlocks.pop_front();
		}

		if (!mReplenishedBlocks.empty())
		{
			return mReplenishedBlocks.front();
		}

		MemoryBlock* block = &mBlocks.emplace_back(mSpec, mResourceTypeInfo);
		mReplenishedBlocks.push_front(block);
		return block;
	}
}

namespace Xuzumi
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// PoolAllocator 
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	PoolAllocator::PoolAllocator(const PoolAllocatorSpecification& spec)
		: mExpirationGuard(this)
		, mSpec(spec)
	{
	}

	void PoolAllocator::Deallocate(void* resource)
	{
		auto chunk = reinterpret_cast<Internal::MemoryChunk*>(resource) - 1;
		
		TypeID resourceTypeID = chunk->ResourceTypeID;
		auto poolIt = mPools.find(resourceTypeID);

		if (mPools.end() == poolIt)
		{
			// TODO: report error.
			return;
		}

		poolIt->second->Deallocate(resource);
	}
}
