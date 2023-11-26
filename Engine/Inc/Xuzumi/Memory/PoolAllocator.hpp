#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Debug/Assertion.hpp"
#include "Xuzumi/Memory/SharedPtr.hpp"
#include "Xuzumi/Memory/FactoryExpirationGuard.hpp"
#include "Xuzumi/Core/Templates/TypeStorage.hpp"

namespace Xuzumi::Internal
{
	template<typename ResourceT>
	struct ResourcePoolChunk
	{
		inline static constexpr std::ptrdiff_t kInvalidChunkIndex = -1;

		TypeStorage<ResourceT> Resource;
		std::ptrdiff_t NextChunk = kInvalidChunkIndex;
	};
}

namespace Xuzumi
{
	template<typename ResourceT>
	class PoolAllocator
	{
	public:
		static_assert(
			!std::is_reference_v<ResourceT>,
			"Xuzumi: reference type is not allowed here"
		);
		static_assert(
			!std::is_array_v<ResourceT>,
			"Xuzumi: array type is not allowed here"
		);

		using ResourceType = ResourceT;
		using PointerType = std::add_pointer_t<ResourceType>;

	private:
		using Chunk = Internal::ResourcePoolChunk<ResourceType>;

	public:
		PoolAllocator(std::size_t size)
			: mChunks(size)
			, mChunksInUse(size)
			, mFreeChunkIndex(Chunk::kInvalidChunkIndex)
			, mPoolExpirationGuard(this)
		{
			SetupFreeList();
		}

		template<typename... ArgsT>
		PointerType Allocate(ArgsT&&... args)
		{
			XZ_ASSERT(!IsExhausted(), "Trying to allocate from an exhausted pool");

			mChunksInUse[mFreeChunkIndex] = true;

			Chunk& chunk = mChunks[mFreeChunkIndex];
			mFreeChunkIndex = chunk.NextChunk;

			return chunk.Resource.Construct(std::forward<ArgsT>(args)...);
		}

		template<typename... ArgsT>
		SharedPtr<ResourceType> AllocateShared(ArgsT&&... args)
		{
			return SharedPtr<ResourceType>(
				Allocate(std::forward<ArgsT>(args)...),
				mPoolExpirationGuard.MakeDangleProtectedDeleter(
					&PoolAllocator::Deallocate
				)
			);
		}

		void Deallocate(PointerType resource)
		{
			if (nullptr == resource)
			{
				XZ_LOG(Warning, "Trying to deallocate a nullptr");
				return;
			}

			auto chunk = reinterpret_cast<Chunk*>(resource);
			std::ptrdiff_t chunkIndex = chunk - &mChunks.front();

			XZ_ASSERT(
				chunkIndex >= 0 && chunkIndex < mChunks.size(),
				"Not a pool chunk"
			);
			XZ_ASSERT(
				mChunksInUse[chunkIndex],
				"Trying to deallocate the memory that is not currently acquired"
			);

			mChunks[chunkIndex].NextChunk = mFreeChunkIndex;
			mFreeChunkIndex = chunkIndex;

			mChunksInUse[chunkIndex] = false;
		
			chunk->Resource.Destruct();
		}

		bool IsExhausted() const
		{
			return Chunk::kInvalidChunkIndex == mFreeChunkIndex;
		}

	private:
		void SetupFreeList()
		{
			std::ptrdiff_t iChunk = mFreeChunkIndex = 0;

			for (; iChunk < mChunks.size() - 1; iChunk++)
			{
				mChunks[iChunk].NextChunk = iChunk + 1;
			}
		}

		std::vector<Chunk> mChunks;
		std::vector<bool> mChunksInUse;
		std::ptrdiff_t mFreeChunkIndex = Chunk::kInvalidChunkIndex;

		Internal::FactoryExpirationGuard<PoolAllocator> mPoolExpirationGuard;
	};
}
