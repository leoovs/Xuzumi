#pragma once

#include "Xuzumi/Memory/SharedPtr.hpp"
#include "Xuzumi/Memory/PointerBase.hpp"
#include "Xuzumi/Memory/Referencer.hpp"
#include "Xuzumi/Core/Templates/IsCompatible.hpp"

namespace Xuzumi
{
	template<typename T>
	class WeakPtr
	{
	public:
		using Traits = Internal::PointerTraits<T>;

		using PointerType = typename Traits::PointerType;

		template<typename OtherT>
		friend class WeakPtr;

		// Contructors.
		WeakPtr() = default;

		WeakPtr(std::nullptr_t)
			: WeakPtr()
		{
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		WeakPtr(const SharedPtr<OtherT>& ptr)
			: mResourcePointer(ptr.mResourcePointer)
			, mReferencer(ptr.mReferencer)
		{
			mReferencer.Weakify();
		}

		WeakPtr(const WeakPtr& other)
			: mResourcePointer(other.mResourcePointer)
			, mReferencer(other.mReferencer)
		{
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		WeakPtr(const WeakPtr<OtherT>& other)
			: mResourcePointer(other.mResourcePointer)
			, mReferencer(other.mReferencer)	
		{
		}

		WeakPtr(WeakPtr&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mReferencer(std::move(other.mReferencer))
		{
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		WeakPtr(WeakPtr<OtherT>&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mReferencer(std::move(other.mReferencer))
		{
		}

		// Assign operators.

		WeakPtr& operator=(const WeakPtr& other)
		{
			if (this == &other)
			{
				return *this;
			}

			mResourcePointer = other.mResourcePointer;
			mReferencer = other.mReferencer;
			
			return *this;
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		WeakPtr& operator=(const WeakPtr<OtherT>& other)
		{
			mResourcePointer = other.mResourcePointer;
			mReferencer = other.mReferencer;

			return *this;
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		WeakPtr& operator=(const SharedPtr<OtherT>& ptr)
		{
			mResourcePointer = ptr.mResourcePointer;
			mReferencer = ptr.mReferencer;
			mReferencer.Weakify();
			
			return *this;
		}

		WeakPtr& operator=(WeakPtr&& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			mResourcePointer = std::exchange(other.mResourcePointer, nullptr);
			mReferencer = std::move(other.mReferencer);

			return *this;
		}
		
		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		WeakPtr& operator=(WeakPtr<OtherT>&& other) noexcept
		{
			mResourcePointer = std::exchange(other.mResourcePointer, nullptr);
			mReferencer = std::move(other.mReferencer);

			return *this;
		}

		// Modifiers.

		void Reset()
		{
			mResourcePointer = nullptr;
			mReferencer.Reset();
		}

		void Swap(WeakPtr& other)
		{
			std::swap(mResourcePointer, other.mResourcePointer);
			std::swap(mReferencer, other.mReferencer);
		}

		// Observers.

		std::uint32_t UseCount() const
		{
			return mReferencer.UseCount();
		}

		bool Expired() const
		{
			return UseCount() == 0;
		}

		SharedPtr<T> Lock() const
		{
			if (Expired())
			{
				return nullptr;
			}
			
			SharedPtr<T> ptr;

			ptr.mResourcePointer = mResourcePointer;
			ptr.mReferencer = mReferencer;	
			ptr.mReferencer.Strongify();

			return ptr;
		}

		explicit operator SharedPtr<T>() const
		{
			return Lock();
		}

	private:
		PointerType mResourcePointer = nullptr;
		Internal::Referencer mReferencer;
	};
}
