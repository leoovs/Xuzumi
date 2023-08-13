#ifndef XUZUMI_MEMORY_UNIQUEPTR_HPP_
#define XUZUMI_MEMORY_UNIQUEPTR_HPP_

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Memory/Deleter.hpp"
#include "Xuzumi/TypeMeta/TypeInfo.hpp"
#include "Xuzumi/Debug/DebugSession.hpp"

namespace Xuzumi::Internal
{
	template<typename T>
	struct UniquePtrTraits
	{
		static_assert(
			!std::is_reference_v<T>,
			"Xuzumi: reference type argument for UniquePtr is not allowed"
		);

		using ValueType = std::remove_extent_t<T>;
		using PointerType = std::add_pointer_t<ValueType>;
		using ReferenceType = std::add_lvalue_reference_t<ValueType>;

		static constexpr bool IsArray()
		{
			return std::is_array_v<T>;
		}
	};

	class IUniquePtrControlBlock
	{
	public:
		virtual ~IUniquePtrControlBlock() = default;

		virtual void Delete() = 0;

		virtual TypeInfo GetResourceTypeInfo() const = 0;
		virtual TypeInfo GetDeleterTypeInfo() const = 0;
	};

	// TODO: create wrapper class called UniqueReferencer.
	template<typename T, typename DeleterT>
	class UniquePtrControlBlock : public IUniquePtrControlBlock
	{
	public:
		using Traits = UniquePtrTraits<T>;
		
		using ValueType = typename Traits::ValueType;
		using PointerType = typename Traits::PointerType;

		UniquePtrControlBlock(PointerType resourcePointer, DeleterT deleter)
			: mResourcePointer(resourcePointer)
			, mDeleter(deleter)
		{
		}

		~UniquePtrControlBlock() override = default;

		void Delete() override
		{
			mDeleter(std::exchange(mResourcePointer, nullptr));
		}

		TypeInfo GetResourceTypeInfo() const override
		{
			return TypeInfo::Get<ValueType>();
		}

		TypeInfo GetDeleterTypeInfo() const override
		{
			return TypeInfo::Get<DeleterT>();
		}

	private:
		PointerType mResourcePointer = nullptr;
		DeleterT mDeleter;
	};
}

namespace Xuzumi
{
	template<typename T>
	class UniquePtr
	{
	public:
		using Traits = Internal::UniquePtrTraits<T>;
	
		using ValueType = typename Traits::ValueType;
		using PointerType = typename Traits::PointerType;
		using ReferenceType = typename Traits::ReferenceType;

		template<typename OtherT>
		friend class UniquePtr;

		UniquePtr() = default;

		UniquePtr(std::nullptr_t)
			: UniquePtr()
		{
		}

		template<typename OtherT>
		explicit UniquePtr(OtherT* pointer)
		{
			if constexpr (Traits::IsArray())
			{
				OvertakeResourceOwnership(
					pointer,
					DefaultDeleter<OtherT[]>()
				);
			}
			else
			{
				OvertakeResourceOwnership(
					pointer,
					DefaultDeleter<OtherT>()
				);
			}
		}

		template<typename OtherT, typename DeleterT>
		explicit UniquePtr(OtherT* pointer, DeleterT deleter)
		{
			OvertakeResourceOwnership(
				pointer,
				deleter
			);
		}

		UniquePtr(const UniquePtr&) = delete;

		UniquePtr(UniquePtr&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mControlBlock(std::exchange(other.mControlBlock, nullptr))
		{
		}

		template<
			typename OtherT,
			typename = std::enable_if<std::is_convertible_v<OtherT, ValueType>>
		>
		UniquePtr(UniquePtr<OtherT>&& other) noexcept
			: mResourcePointer(
				static_cast<PointerType>(
					std::exchange(other.mResourcePointer, nullptr)
				)
			),
			mControlBlock(std::exchange(other.mControlBlock, nullptr))
		{
		}

		~UniquePtr()
		{
			CleanUp();
		}

		UniquePtr& operator=(const UniquePtr& other) = delete;

		UniquePtr& operator=(UniquePtr&& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			CleanUp();

			mResourcePointer = std::exchange(other.mResourcePointer, nullptr);
			mControlBlock = std::exchange(other.mControlBlock, nullptr);
		
			return *this;
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<std::is_convertible_v<OtherT, ValueType>>
		>
		UniquePtr& operator=(UniquePtr<OtherT>&& other) noexcept
		{
			CleanUp();

			mResourcePointer = static_cast<PointerType>(
				std::exchange(other.mResourcePointer, nullptr)
			);
			mControlBlock = std::exchange(other.mControlBlock, nullptr);

			return *this;
		}

		UniquePtr& operator=(std::nullptr_t)
		{
			Reset();
			return *this;
		}

		PointerType Release()
		{
			PointerType ownedResource = std::exchange(mResourcePointer, nullptr);
			DeleteControlBlock();

			return ownedResource;
		}

		// TODO: make Reset take OtherT* pointer. Make Reset contrained.
		void Reset()
		{
			CleanUp();
		}

		void Reset(PointerType pointer)
		{
			Reset(pointer, DefaultDeleter<T>());
		}

		template<typename DeleterT>
		void Reset(PointerType pointer, DeleterT deleter)
		{
			CleanUp();	
			OvertakeResourceOwnership(
				pointer,
				deleter
			);
		}

		void Swap(UniquePtr& other)
		{
			if (this == &other)
			{
				return;
			}

			std::swap(mResourcePointer, other.mResourcePointer);
			std::swap(mControlBlock, other.mControlBlock);
		}

		template<typename OtherT>
		UniquePtr<OtherT> AsUnsafe()
		{
			UniquePtr<OtherT> other;

			other.mResourcePointer = reinterpret_cast<
				typename decltype(other)::PointerType
			>(std::exchange(mResourcePointer, nullptr));

			other.mControlBlock = std::exchange(mControlBlock, nullptr);

			return other;
		}

		template<typename OtherT>
		UniquePtr<OtherT> As()
		{
			using OtherValueType = std::remove_extent_t<OtherT>;

			if (TypeInfo::Get<OtherValueType>() == GetResourceTypeInfo())
			{
				return AsUnsafe<OtherT>();
			}

			return nullptr;
		}

		PointerType Get() const
		{
			return mResourcePointer;
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
		
		ReferenceType operator*() const
		{
			XZ_ASSERT(*this, "Trying to dereference a nullptr");
			return *Get();
		}

		PointerType operator->() const
		{
			XZ_ASSERT(*this, "Trying to dereference a nullptr");
			return Get();
		}

		ReferenceType operator[](std::ptrdiff_t index) const
		{
			XZ_ASSERT(*this, "Trying to dereference a nullptr");
			return Get()[index];
		}

		bool operator==(const UniquePtr& other) const
		{
			return this == &other || mResourcePointer == other.mResourcePointer;
		}

		bool operator==(std::nullptr_t)
		{
			return bool(*this);
		}

		bool operator!=(const UniquePtr& other) const
		{
			return mResourcePointer != other.mResourcePointer;
		}

		bool operator!=(std::nullptr_t)
		{
			return !(*this == nullptr);
		}

		explicit operator bool() const
		{
			return bool(Get());
		}

	private:
		template<
			typename OtherT,
			typename DeleterT,
			typename = std::enable_if<std::is_convertible_v<OtherT*, T*>>
		>
		void OvertakeResourceOwnership(
			OtherT* pointer,
			DeleterT deleter
		)
		{
			mResourcePointer = static_cast<PointerType>(pointer);
			
			mControlBlock = new Internal::UniquePtrControlBlock<OtherT, DeleterT>(
				pointer,
				deleter
			);
		}

		void DeleteResource()
		{
			mResourcePointer = nullptr;
			if (mControlBlock)
			{
				mControlBlock->Delete();
			}
		}

		void DeleteControlBlock()
		{
			if (mControlBlock)
			{
				delete mControlBlock;
				mControlBlock = nullptr;
			}
		}

		void CleanUp()
		{
			DeleteResource();
			DeleteControlBlock();
		}

		PointerType mResourcePointer = nullptr;
		Internal::IUniquePtrControlBlock* mControlBlock = nullptr;
	};

	template<typename T, typename... ArgsT>
	std::enable_if_t<!std::is_array_v<T>, UniquePtr<T>>
	MakeUnique(ArgsT&&... args)
	{
		return UniquePtr<T>(new T{ std::forward<ArgsT>(args)... });
	}

	template<typename T>
	std::enable_if_t<std::is_array_v<T> && std::extent_v<T> == 0, UniquePtr<T>>
	MakeUnique(std::size_t size)
	{
		return UniquePtr<T>(new std::remove_extent_t<T>[size]{});
	}
}

#endif // XUZUMI_MEMORY_UNIQUEPTR_HPP_
