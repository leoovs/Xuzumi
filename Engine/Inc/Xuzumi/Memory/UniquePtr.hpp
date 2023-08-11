#ifndef XUZUMI_MEMORY_UNIQUEPTR_HPP_
#define XUZUMI_MEMORY_UNIQUEPTR_HPP_

#include "Xuzumi/Precompiled.hpp"
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

	template<typename T, typename DeleterT>
	class UniquePtrControlBlock : public IUniquePtrControlBlock
	{
	public:
		using Traits = UniquePtrTraits<T>;
		
		using ValueType = typename Traits::ValueType;
		using PointerType = typename Traits::PointerType;
		using ReferenceType = typename Traits::ReferenceType;

		UniquePtrControlBlock(PointerType resourcePointer, DeleterT deleter)
			: mResourcePointer(resourcePointer)
			, mDeleter(deleter)
		{
		}

		~UniquePtrControlBlock() override = default;

		void Delete() override
		{
			mDeleter(mResourcePointer);
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

	template<typename T>
	class UniquePtrDefaultDeleter
	{
	public:
		using Traits = UniquePtrTraits<T>;
		using PointerType = typename Traits::PointerType;
	
		void operator()(PointerType resourcePointer)
		{
			if (Traits::IsArray())
			{
				delete[] resourcePointer;
			}
			else
			{
				delete resourcePointer;
			}
		}
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

		explicit UniquePtr(PointerType pointer)
			: UniquePtr(pointer, Internal::UniquePtrDefaultDeleter<T>())
		{
		}

		template<typename DeleterT>
		explicit UniquePtr(PointerType pointer, DeleterT deleter)
		{
			OvertakeResourceOwnership(pointer, deleter);
		}

		UniquePtr(const UniquePtr&) = delete;

		UniquePtr(UniquePtr&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mControlBlock(std::exchange(other.mControlBlock, nullptr))
		{
		}

		template<typename OtherT>
		UniquePtr(UniquePtr<OtherT>&& other) noexcept
			: mResourcePointer(
				static_cast<PointerType>(
					std::exchange(other.mResourcePointer, nullptr)
				)
			),
			mControlBlock(std::exchange(other.mControlBlock, nullptr))
		{
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

		template<typename OtherT>
		UniquePtr& operator=(UniquePtr<OtherT>&& other) noexcept
		{
			// TODO: should we do the `this == &other` check here???
		
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

		~UniquePtr()
		{
			CleanUp();
		}

		PointerType Get() const
		{
			return mResourcePointer;
		}

		void Reset()
		{
			CleanUp();
		}

		void Reset(PointerType pointer)
		{
			Reset(pointer, Internal::UniquePtrDefaultDeleter<T>());
		}

		template<typename DeleterT>
		void Reset(PointerType pointer, DeleterT deleter)
		{
			Reset();	
			OvertakeResourceOwnership(pointer, deleter);
		}

		PointerType Release()
		{
			PointerType ownedResource = std::exchange(mResourcePointer, nullptr);
			DeleteControlBlock();

			return ownedResource;
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
		UniquePtr<OtherT> Mimicry()
		{
			UniquePtr<OtherT> other;

			other.mResourcePointer = reinterpret_cast<
				typename decltype(other)::PointerType
			>(std::exchange(mResourcePointer, nullptr));

			other.mControlBlock = std::exchange(mControlBlock, nullptr);
		
			return other;
		}

		template<typename OtherT>
		UniquePtr<OtherT> MimicrySafe()
		{
			if (TypeInfo::Get<OtherT>() == GetResourceTypeInfo())
			{
				return Mimicry<OtherT>();
			}
		
			return nullptr;
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

		PointerType operator->() const
		{
			XZ_ASSERT(*this, "Trying to dereference a nullptr");
			return Get();
		}

		ReferenceType operator*() const
		{
			XZ_ASSERT(*this, "Trying to dereference a nullptr");
			return *Get();
		}

		ReferenceType operator[](std::ptrdiff_t index) const
		{
			XZ_ASSERT(*this, "Trying to dereference a nullptr");
			return Get()[index];
		}

		explicit operator bool() const
		{
			return bool(Get());
		}

	private:
		template<typename DeleterT>
		void OvertakeResourceOwnership(PointerType pointer, DeleterT deleter)
		{
			mControlBlock = new Internal::UniquePtrControlBlock<T, DeleterT>(
				mResourcePointer = pointer,
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

	template<typename T, typename... ArgsT>
	std::enable_if_t<std::is_array_v<T> && std::extent_v<T> == 0, UniquePtr<T>>
	MakeUnique(std::size_t size)
	{
		return UniquePtr<T>(new std::remove_extent_t<T>[size]{});
	}
}

#endif // XUZUMI_MEMORY_UNIQUEPTR_HPP_
