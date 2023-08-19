#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/TypeMeta/TypeInfo.hpp"

namespace Xuzumi::Internal
{
	class IControlBlock
	{
	public:
		virtual ~IControlBlock() = default;

		virtual std::uint32_t GetStrongRefs() const = 0;
	
		virtual void IncrementStrongRefs() = 0;
		virtual void IncrementWeakRefs() = 0;

		virtual void DecrementStrongRefs() = 0;
		virtual void DecrementWeakRefs() = 0;
	
		virtual bool ResourceIsAlive() const = 0;
		virtual bool HasNoWeakRefs() const = 0;

		virtual TypeInfo GetResourceTypeInfo() const = 0;
	};

	template<typename T, typename DeleterT>
	class ReferencingControlBlock : public IControlBlock
	{
	public:
		static_assert(
			!std::is_reference_v<T>,
			"Xuzumi: pointer to a reference is not allowed"
		);

		using ValueType = std::remove_extent_t<T>;
		using PointerType = std::add_pointer_t<ValueType>;

		ReferencingControlBlock(PointerType resourcePointer, DeleterT deleter)
			: mResourcePointer(resourcePointer)
			, mDeleter(deleter)
		{
			mStrongRefs.store(1);
			mWeakRefs.store(0);
		}

		std::uint32_t GetStrongRefs() const override
		{
			return mStrongRefs.load();
		}

		void IncrementStrongRefs() override
		{
			mStrongRefs.fetch_add(1);
		}

		void IncrementWeakRefs() override
		{
			mWeakRefs.fetch_add(1);
		}

		void DecrementStrongRefs() override
		{
			if (!ResourceIsAlive())
			{
				return;
			}

			mStrongRefs.fetch_sub(1);
		
			if (mStrongRefs.load() == 0)
			{
				mDeleter(std::exchange(mResourcePointer, nullptr));
			}
		}

		void DecrementWeakRefs() override
		{
			mWeakRefs.fetch_sub(1);
		}

		bool ResourceIsAlive() const override
		{
			return bool(mResourcePointer);
		}

		bool HasNoWeakRefs() const override
		{
			return mWeakRefs.load() == 0;
		}

		TypeInfo GetResourceTypeInfo() const override
		{
			return TypeInfo::Get<T>();
		}

	private:
		PointerType mResourcePointer = nullptr;
		DeleterT mDeleter;
		std::atomic_uint32_t mStrongRefs;
		std::atomic_uint32_t mWeakRefs;
	};
}
