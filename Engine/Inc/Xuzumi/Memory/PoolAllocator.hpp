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
	/**
	 * @brief Implements a memory allocator based on memory-pools.
	 * 
	 * Applies an efficient allocation strategy when allocating many objects of
	 * the same type.
	 * 
	 * @tparam ResourceT A type of the memory resource to allocate.
	 */
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

		/**
		 * @brief The type of the memory resource.
		 */
		using ResourceType = ResourceT;

		/**
		 * @brief The type of the pointer to the memory resource. 
		 */
		using PointerType = std::add_pointer_t<ResourceType>;

	private:
		using Chunk = Internal::ResourcePoolChunk<ResourceType>;

	public:
		/**
		 * @brief Constructs a pool that can fit @p size memory resources.
		 * 
		 * @param size The amount of memory resources pool can allocate.
		 */
		PoolAllocator(std::size_t size)
			: mChunks(size)
			, mChunksInUse(size)
			, mFreeChunkIndex(Chunk::kInvalidChunkIndex)
			, mPoolExpirationGuard(this)
		{
			SetupFreeList();
		}

		/**
		 * @brief Allocate a memory resource from a pool.
		 * 
		 * Allocates and constructs a memory resource. The allocated resource must
		 * be freed with the `Deallocate` method.
		 * 
		 * @tparam ArgsT Types of the arguments that are passed to the memory
		 * resource constructor.
		 * 
		 * @param args The arguments passed to the memory resource constructor.
		 * 
		 * @return A raw pointer to the memory resource.
		 */
		template<typename... ArgsT>
		PointerType Allocate(ArgsT&&... args)
		{
			XZ_ASSERT(!IsExhausted(), "Trying to allocate from an exhausted pool");

			mChunksInUse[mFreeChunkIndex] = true;

			Chunk& chunk = mChunks[mFreeChunkIndex];
			mFreeChunkIndex = chunk.NextChunk;

			return chunk.Resource.Construct(std::forward<ArgsT>(args)...);
		}

		/**
		 * @brief Allocate a memory resource from a pool and wrap it into
		 * `SharedPtr`. 
		 * 
		 * Internally calls `Allocate` and wraps the resulting pointer into the
		 * `SharedPtr`. Therefore, the `Deallocate` method is called automatically
		 * when the smart pointer reference count reaches zero.
		 * 
		 * @tparam ArgsT Types of the arguments that are passed to the memory
		 * resource constructor.
		 * 
		 * @param args The arguments passed to the memory resource constructor.
		 * 
		 * @return A smart pointer `SharedPtr<ResourceType>` to the memory resource.
		 */
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

		/**
		 * @brief Destructs and deallocates the memory resource.
		 * 
		 * Calls the destructor of the memory resource and marks its memory as free
		 * by adding it to a free-list. The freed memory may be reused later when 
		 * another allocation occurs.
		 * 
		 * @param resource A raw pointer to the memory resource.
		 */
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

		/**
		 * @brief Check whether the `*this` is out of memory.
		 * 
		 * @retval true There is not memory left in the `*this` to allocate more
		 * memory resources.
		 * @retval false The `*this` can allocate more memory resources from its
		 * pool.
		 */
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
