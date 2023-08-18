#include "Xuzumi/Memory/StrongReferencer.hpp"

namespace Xuzumi::Internal
{
	StrongReferencer::StrongReferencer(const StrongReferencer& other)
		: mControlBlock(other.mControlBlock)
	{
		IncrementRefs();
	}

	StrongReferencer::StrongReferencer(StrongReferencer&& other) noexcept
		: mControlBlock(std::exchange(other.mControlBlock, nullptr))
	{
	}

	StrongReferencer::~StrongReferencer()
	{
		Reset();
	}

	StrongReferencer& StrongReferencer::operator=(const StrongReferencer& other)
	{
		if (this == &other)
		{
			return *this;
		}

		DecrementRefs();
		mControlBlock = other.mControlBlock;
		IncrementRefs();

		return *this;
	}

	StrongReferencer& StrongReferencer::operator=(StrongReferencer&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DecrementRefs();
		mControlBlock = std::exchange(other.mControlBlock, nullptr);

		return *this;
	}

	std::uint32_t StrongReferencer::UseCount() const
	{
		return mControlBlock
			? mControlBlock->GetStrongRefs()
			: 0;
	}

	TypeInfo StrongReferencer::GetResourceTypeInfo() const
	{
		return mControlBlock
			? mControlBlock->GetResourceTypeInfo()
			: TypeInfo();
	}

	void StrongReferencer::Reset()
	{
		DecrementRefs();
		mControlBlock = nullptr;
	}

	void StrongReferencer::IncrementRefs()
	{
		if (mControlBlock)
		{
			mControlBlock->IncrementStrongRefs();
		}
	}
	
	void StrongReferencer::DecrementRefs()
	{
		if (!mControlBlock)
		{
			return;	
		}

		mControlBlock->DecrementStrongRefs();

		if (!mControlBlock->ResourceIsAlive() && mControlBlock->HasNoWeakRefs())
		{
			delete mControlBlock;
			mControlBlock = nullptr;
		}
	}
}
