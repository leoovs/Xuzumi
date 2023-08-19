#include "Xuzumi/Memory/Referencer.hpp"

namespace Xuzumi::Internal
{
	Referencer::Referencer(const Referencer& other)
		: mControlBlock(other.mControlBlock)
		, mMode(other.mMode)
	{
		IncrementRefs();
	}

	Referencer::Referencer(Referencer&& other) noexcept
		: mControlBlock(std::exchange(other.mControlBlock, nullptr))
		, mMode(other.mMode)
	{
	}

	Referencer::~Referencer()
	{
		DecrementRefs();
	}

	Referencer& Referencer::operator=(const Referencer& other)
	{
		if (this == &other)
		{
			return *this;
		}

		DecrementRefs();
		mControlBlock = other.mControlBlock;
		mMode = other.mMode;
		IncrementRefs();	

		return *this;
	}

	Referencer& Referencer::operator=(Referencer&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DecrementRefs();
		mControlBlock = std::exchange(other.mControlBlock, nullptr);
		mMode = other.mMode;

		return *this;
	}

	std::uint32_t Referencer::UseCount() const
	{
		return mControlBlock
			? mControlBlock->GetStrongRefs()
			: 0;
	}

	TypeInfo Referencer::GetResourceTypeInfo() const
	{
		return mControlBlock
			? mControlBlock->GetResourceTypeInfo()
			: TypeInfo();
	}

	void Referencer::Reset()
	{
		DecrementRefs();
		mControlBlock = nullptr;
	}

	void Referencer::Weakify()
	{
		SetMode(ReferencerMode::Weak);
	}

	void Referencer::Strongify()
	{
		SetMode(ReferencerMode::Strong);
	}

	void Referencer::IncrementRefs()
	{
		if (!mControlBlock)
		{
			return;
		}

		switch (mMode)
		{
		case ReferencerMode::Strong:
			mControlBlock->IncrementStrongRefs();
			break;

		case ReferencerMode::Weak:
			mControlBlock->IncrementWeakRefs();
			break;
		}
	}

	void Referencer::DecrementRefs()
	{
		if (!mControlBlock)
		{
			return;
		}

		switch (mMode)
		{
		case ReferencerMode::Strong:
			mControlBlock->DecrementStrongRefs();
			break;

		case ReferencerMode::Weak:
			mControlBlock->DecrementWeakRefs();
			break;
		}

		if (!mControlBlock->ResourceIsAlive() && mControlBlock->HasNoWeakRefs())
		{
			delete mControlBlock;
			mControlBlock = nullptr;
		}
	}

	void Referencer::SetMode(ReferencerMode mode)
	{
		if (!mControlBlock || mMode == mode)
		{
			return;
		}

		switch (mMode = mode)
		{
		case ReferencerMode::Strong:
			mControlBlock->IncrementStrongRefs();
			mControlBlock->DecrementWeakRefs();
			break;

		case ReferencerMode::Weak:
			mControlBlock->IncrementWeakRefs();
			mControlBlock->DecrementStrongRefs();
			break;
		}
	}
}
