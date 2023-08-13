#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Memory/Deleter.hpp"
#include "Xuzumi/TypeMeta/TypeInfo.hpp"

namespace Xuzumi::Internal
{
	template<typename T>
	struct SharedPtrTraits
	{
		static_assert(
			!std::is_reference_v<T>,
			"Xuzumi: reference type argument for SharedPtr is not allowed"
		);
	
		using ValueType = std::remove_extent_t<T>;
		using PointerType = std::add_pointer_t<ValueType>;
		using ReferenceType = std::add_lvalue_reference_t<ValueType>;
	
		static constexpr bool IsArray()
		{
			return std::is_array_v<T>;
		}
	};

	// TODO: create wrapper class called SharedReferencer.
	class IRefCountedControlBlock
	{
	public:
		virtual ~IRefCountedControlBlock() = default;
	
		virtual void IncrementStrong() = 0;
		virtual void DecrementStrong() = 0;
		virtual std::uint32_t GetStrongReferences() const = 0;
		virtual bool HasSingleStrongReference() const = 0;

		virtual void IncrementWeak() = 0;
		virtual void DecrementWeak() = 0;
		virtual bool HasNoWeakReferences() const = 0;
	
		virtual TypeInfo GetResourceTypeInfo() const = 0;
		virtual TypeInfo GetDeleterTypeInfo() const = 0;
	
		virtual bool ResourceIsAlive() const = 0;
	};

	template<typename T, typename DeleterT>
	class RefCountedControlBlock : public IRefCountedControlBlock
	{
	public:
		using Traits = SharedPtrTraits<T>;

		using ValueType = typename Traits::ValueType;
		using PointerType = typename Traits::PointerType;

		RefCountedControlBlock(PointerType resourcePointer, DeleterT deleter)
			: mResourcePointer(resourcePointer)
			, mDeleter(deleter)
		{
			mStrongReferences.store(1);
			mWeakReferences.store(0);
		}

		void IncrementStrong() override
		{
			if (ResourceIsAlive())
			{
				mStrongReferences.fetch_add(1);
			}
		}

		void DecrementStrong() override
		{
			if (!ResourceIsAlive())
			{
				return;
			}

			mStrongReferences.fetch_sub(1);
			
			if (mStrongReferences.load() == 0)
			{
				mDeleter(std::exchange(mResourcePointer, nullptr));
				mStrongReferences.store(0);
				return;
			}
		}

		std::uint32_t GetStrongReferences() const override
		{
			return mStrongReferences.load();
		}

		bool HasSingleStrongReference() const override
		{
			return mStrongReferences.load() == 1;
		}

		void IncrementWeak() override
		{
			mWeakReferences.fetch_add(1);
		}

		void DecrementWeak() override
		{
			mWeakReferences.fetch_sub(1);
		}

		bool HasNoWeakReferences() const override
		{
			return mWeakReferences.load() == 0;
		}

		TypeInfo GetResourceTypeInfo() const override
		{
			return TypeInfo::Get<ValueType>();
		}

		TypeInfo GetDeleterTypeInfo() const override
		{
			return TypeInfo::Get<DeleterT>();
		}

		bool ResourceIsAlive() const override
		{
			return bool(mResourcePointer);
		}

	private:
		PointerType mResourcePointer = nullptr;
		DeleterT mDeleter;
		std::atomic_uint32_t mStrongReferences;
		std::atomic_uint32_t mWeakReferences;
	};
}

namespace Xuzumi
{
	template<typename T>
	class SharedPtr
	{
	public:
		using Traits = Internal::SharedPtrTraits<T>;

		using ValueType = typename Traits::ValueType;
		using PointerType = typename Traits::PointerType;
		using ReferenceType = typename Traits::ReferenceType;

		template<typename OtherT>
		friend class SharedPtr;
		
		// Constructors.
		SharedPtr() = default;

		SharedPtr(std::nullptr_t)
			: SharedPtr() 
		{
		}

		template<typename OtherT>
		SharedPtr(OtherT* pointer)
		{
			if constexpr (Traits::IsArray())
			{
				UndertakeResourceOwnership(pointer, DefaultDeleter<OtherT[]>());		
			}
			else
			{
				UndertakeResourceOwnership(pointer, DefaultDeleter<OtherT>());		
			}
		}

		template<
			typename OtherT,
			typename DeleterT,
			typename = std::enable_if_t<std::is_convertible_v<OtherT*, PointerType>>
		>
		SharedPtr(OtherT* pointer, DeleterT deleter)
		{
			UndertakeResourceOwnership(pointer, deleter);
		}

		SharedPtr(const SharedPtr& other)
			: mResourcePointer(other.mResourcePointer)
			, mControlBlock(other.mControlBlock)
		{
			ShareResourceOwnership();
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<std::is_convertible_v<OtherT, ValueType>>
		>
		SharedPtr(const SharedPtr<OtherT>& other)
			: mResourcePointer(static_cast<OtherT*>(other.mResourcePointer))
			, mControlBlock(other.mControlBlock)
		{
		}

		SharedPtr(SharedPtr&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mControlBlock(std::exchange(other.mControlBlock, nullptr))
		{
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<std::is_convertible_v<OtherT, ValueType>>
		>
		SharedPtr(SharedPtr<OtherT>&& other) noexcept
			: mResourcePointer(
				static_cast<OtherT*>(
					std::exchange(other.mResourcePointer, nullptr)
				)
			)
			, mControlBlock(std::exchange(other.mControlBlock, nullptr))
		{
		}

		// Destructor.
		~SharedPtr()
		{
			GiveUpResourceOwnership();
		}

		// Assignment operators.
		SharedPtr& operator=(const SharedPtr& other)
		{
			if (this == &other)
			{
				return *this;
			}

			GiveUpResourceOwnership();
			mResourcePointer = other.mResourcePointer;
			mControlBlock = other.mControlBlock;
			ShareResourceOwnership();

			return *this;
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<std::is_convertible_v<OtherT, ValueType>>
		>
		SharedPtr& operator=(const SharedPtr<OtherT>& other)
		{
			GiveUpResourceOwnership();
			mResourcePointer = static_cast<PointerType>(other.mResourcePointer);
			mControlBlock = other.mControlBlock;
			ShareResourceOwnership();

			return *this;
		}

		SharedPtr& operator=(SharedPtr&& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			GiveUpResourceOwnership();
			mResourcePointer = std::exchange(other.mResourcePointer, nullptr);
			mControlBlock = std::exchange(other.mControlBlock, nullptr);

			return *this;
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<std::is_convertible_v<OtherT, ValueType>>
		>
		SharedPtr& operator=(SharedPtr<OtherT>&& other) noexcept
		{
			GiveUpResourceOwnership();
			mResourcePointer = static_cast<PointerType>(
				std::exchange(other.mResourcePointer, nullptr)
			);
			mControlBlock = std::exchange(other.mControlBlock, nullptr);

			return *this;
		}
		
		// Modifiers.
		void Reset()
		{
			GiveUpResourceOwnership();
		}

		void Reset(std::nullptr_t)
		{
			Reset();
		}

		template<typename OtherT>
		void Reset(OtherT* pointer)
		{
			if constexpr (Traits::IsArray())
			{
				Reset(pointer, DefaultDeleter<OtherT[]>());
			}
			else
			{
				Reset(pointer, DefaultDeleter<OtherT>());
			}
		}

		template<
			typename OtherT,
			typename DeleterT,
			typename = std::enable_if_t<
				std::is_convertible_v<OtherT*, PointerType>
			>
		>
		void Reset(OtherT* pointer, DeleterT deleter)
		{
			GiveUpResourceOwnership();
			UndertakeResourceOwnership(
				static_cast<PointerType>(pointer),
				deleter
			);
		}

		void Swap(SharedPtr& other)
		{
			std::swap(mResourcePointer, other.mResourcePointer);
			std::swap(mControlBlock, other.mControlBlock);
		}

		// Observers.
		PointerType Get() const
		{
			return mResourcePointer;
		}

		std::uint32_t UseCount() const
		{
			if (mControlBlock)
			{
				return mControlBlock->GetStrongReferences();
			}
			return 0;
		}

		TypeInfo GetResourceTypeInfo() const
		{
			if (mControlBlock)
			{
				return mControlBlock->GetResourceTypeInfo();
			}
			return TypeInfo();
		}

		TypeInfo GetDeleterTypeInfo() const
		{
			if (mControlBlock)
			{
				return mControlBlock->GetDeleterTypeInfo();
			}
			return TypeInfo();
		}

		template<typename OtherT>
		SharedPtr<OtherT> AsUnsafe() const
		{
			SharedPtr<OtherT> other;
			
			other.mControlBlock = mControlBlock;
			other.mResourcePointer = reinterpret_cast<
				typename decltype(other)::PointerType
			>(mResourcePointer);
			other.ShareResourceOwnership();

			return other;
		}
		
		template<typename OtherT>
		SharedPtr<OtherT> As() const
		{
			using OtherValueType = std::remove_extent_t<OtherT>;

			if (TypeInfo::Get<OtherValueType>() == GetResourceTypeInfo())
			{
				return AsUnsafe<OtherT>();
			}
			return nullptr;
		}

		ReferenceType operator*() const
		{
			return *Get();
		}

		PointerType operator->() const
		{
			return Get();
		}

		ReferenceType operator[](std::ptrdiff_t index) const
		{
			return Get()[index];
		}

		explicit operator bool() const
		{
			return bool(Get());
		}

		// Other operators.

	private:
		template<typename OtherT, typename DeleterT>
		void UndertakeResourceOwnership(OtherT* pointer, DeleterT deleter)
		{
			mResourcePointer = static_cast<PointerType>(pointer);
			mControlBlock = new Internal::RefCountedControlBlock<OtherT, DeleterT>(
				pointer,
				deleter
			);
		}

		void ShareResourceOwnership()
		{
			if (mControlBlock)
			{
				mControlBlock->IncrementStrong();
			}
		}

		void GiveUpResourceOwnership()
		{
			mResourcePointer = nullptr;
			if (mControlBlock)
			{
				TryDeleteControlBlock();
			}
		}

		void TryDeleteControlBlock()
		{
			mControlBlock->DecrementStrong();
			if (!mControlBlock->ResourceIsAlive()
				&& mControlBlock->HasNoWeakReferences())
			{
				delete mControlBlock;
				mControlBlock = nullptr;
			}
			mControlBlock = nullptr;
		}

		PointerType mResourcePointer = nullptr;
		Internal::IRefCountedControlBlock* mControlBlock = nullptr;
	};

	template<typename T,typename... ArgsT>
	std::enable_if_t<!std::is_array_v<T>, SharedPtr<T>>
	MakeShared(ArgsT&&... args)
	{
		return SharedPtr<T>(new T{ std::forward<ArgsT>(args)... });	
	}

	template<typename T>
	std::enable_if_t<std::is_array_v<T> && std::extent_v<T> == 0, SharedPtr<T>>
	MakeShared(std::size_t size)
	{
		return SharedPtr<T>(new std::remove_extent_t<T>[size]{});
	}
}
