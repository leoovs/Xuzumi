#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/TypeMeta/TypeID.hpp"
#include "Xuzumi/TypeMeta/TypeInfo.hpp"
#include "Xuzumi/Memory/SharedPtr.hpp"
#include "Xuzumi/Memory/FactoryExpirationGuard.hpp"

namespace Xuzumi
{
	struct PoolAllocatorSpecification
	{
		std::size_t BlockSize = 10;
	};
}

namespace Xuzumi::Internal
{
	struct MemoryChunk
	{
		TypeID ResourceTypeID = kInvalidTypeID;
		std::size_t ParentBlockIndex = 0;
		void* ResourceMemory = nullptr;
	};

	class MemoryBlock
	{
	public:
		MemoryBlock(
			const PoolAllocatorSpecification& spec,
			TypeInfo resourceTypeInfo
		);

		MemoryChunk* AcquireChunk();
		void DisposeChunk(MemoryChunk* chunk);

		bool IsExhausted() const;

	private:
		using ChunkVectorStack = std::vector<MemoryChunk*>;

		void AllocateMemory();

		PoolAllocatorSpecification mSpec;
		TypeInfo mResourceTypeInfo;

		std::shared_ptr<void> mMemory;

		ChunkVectorStack mAvailableChunks;
	};

	class MemoryPool
	{
	public:
		MemoryPool(
			const PoolAllocatorSpecification& spec,
			TypeInfo resourceTypeInfo
		);

		MemoryChunk* AcquireChunk();
		void DisposeChunk(MemoryChunk* chunk);

	private:
		MemoryBlock* GetActiveBlock();

		PoolAllocatorSpecification mSpec;
		TypeInfo mResourceTypeInfo;

		std::vector<MemoryBlock> mBlocks;
		std::deque<MemoryBlock*> mReplenishedBlocks;
	};

	template<typename ResourceT>
	inline constexpr bool CanBePoolAllocatorResource
		= !std::is_reference_v<ResourceT>;

	class IObjectPool
	{
	public:
		virtual ~IObjectPool() = default;

		virtual void Deallocate(void* resource) = 0;
	};

	template<
		typename ResourceT,
		typename = std::enable_if_t<CanBePoolAllocatorResource<ResourceT>>
	>
	class ObjectPool : public IObjectPool
	{
	public:
		using ResourcePointer = ResourceT*;

		ObjectPool(const PoolAllocatorSpecification& spec)
			: mSpec(spec)
			, mMemory(spec, TypeInfo::Get<ResourceT>())
		{
		}

		template<typename... ArgsT>
		ResourcePointer Allocate(ArgsT&&... args)
		{
			MemoryChunk* chunk = mMemory.AcquireChunk();

			return new (chunk->ResourceMemory) ResourceT
			{
				std::forward<ArgsT>(args)...
			};
		}

		void Deallocate(void* resource)
		{
			auto chunk = reinterpret_cast<MemoryChunk*>(resource) - 1;
			TypeInfo::Get<ResourceT>().Destruct(chunk->ResourceMemory);
			mMemory.DisposeChunk(chunk);
		}

	private:
		PoolAllocatorSpecification mSpec;
		MemoryPool mMemory;
	};
}

namespace Xuzumi
{
	class PoolAllocator
	{
	public:
		PoolAllocator(const PoolAllocatorSpecification& spec = {});

		template<
			typename ResourceT,
			typename = std::enable_if_t<
				Internal::CanBePoolAllocatorResource<ResourceT>
				&& !std::is_array_v<ResourceT>
			>,
			typename... ArgsT
		>
		ResourceT* Allocate(ArgsT&&... args)
		{
			return GetPool<ResourceT>()->Allocate(std::forward<ArgsT>(args)...);
		}

		template<
			typename ResourceT,
			typename = std::enable_if_t<
				Internal::CanBePoolAllocatorResource<ResourceT>
				&& !std::is_array_v<ResourceT>
			>,
			typename... ArgsT
		>
		SharedPtr<ResourceT> AllocateShared(ArgsT&&... args)
		{
			return SharedPtr<ResourceT>(
				Allocate<ResourceT>(std::forward<ArgsT>(args)...),
				mExpirationGuard.MakeDangleProtectedDeleter(
					&PoolAllocator::DeallocateShared<ResourceT>
				)
			);
		}

		void Deallocate(void* resource);

	private:
		template<typename ResourceT>
		void DeallocateShared(ResourceT* resource)
		{
			Deallocate(resource);
		}

		template<typename ResourceT>
		std::shared_ptr<Internal::ObjectPool<ResourceT>> GetPool()
		{
			TypeID resourceTypeID = GetTypeID<ResourceT>();

			auto poolIt = mPools.find(resourceTypeID);
			if (mPools.end() == poolIt)
			{
				auto pool = std::make_shared<Internal::ObjectPool<ResourceT>>(mSpec);
				mPools[resourceTypeID] = pool;

				return pool;
			}

			return std::static_pointer_cast<
				Internal::ObjectPool<ResourceT>
			>(poolIt->second);
		}

		Internal::FactoryExpirationGuard<PoolAllocator> mExpirationGuard;

		PoolAllocatorSpecification mSpec;
		std::unordered_map<TypeID, std::shared_ptr<Internal::IObjectPool>> mPools;
	};
}
