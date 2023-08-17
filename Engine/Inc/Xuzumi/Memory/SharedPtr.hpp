#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Debug/Assertion.hpp"
#include "Xuzumi/TypeMeta/TypeInfo.hpp"
#include "Xuzumi/Memory/Deleter.hpp"
#include "Xuzumi/Memory/ControlBlock.hpp"
#include "Xuzumi/Core/Templates/IsCompatible.hpp"

namespace Xuzumi::Internal
{
	template<typename T>
	struct SharedPtrTraits
	{
		static_assert(
			!std::is_reference_v<T>,
			"Xuzumi: reference type is not allowed in SharedPtr typename argument"
		);
	
		using ValueType = std::remove_extent_t<T>;
		using PointerType = std::add_pointer_t<ValueType>;
		using ReferenceType = std::add_lvalue_reference_t<ValueType>;
	
		template<typename OtherT, bool IsArrayT>
		struct OtherTypeImpl {};
	
		template<typename OtherT>
		struct OtherTypeImpl<OtherT, true>
		{
			using Type = OtherT[];
		};
	
		template<typename OtherT>
		struct OtherTypeImpl<OtherT, false>
		{
			using Type = OtherT;
		};

		template<typename OtherT>
		using OtherType =
			typename OtherTypeImpl<OtherT, std::is_array_v<T>>::Type;
	};

	class ISharedControlBlock
	{
	public:
		virtual ~ISharedControlBlock() = default;

		virtual std::uint32_t GetStrongRefs() const = 0;
		virtual void IncrementStrongRefs() = 0;
		virtual void DecrementStrongRefs() = 0;

		virtual bool HasNoWeakRefs() const = 0;
		virtual void IncrementWeakRefs() = 0;
		virtual void DecrementWeakRefs() = 0;

		virtual bool ResourceIsAlive() const = 0;

		virtual TypeInfo GetResourceTypeInfo() const = 0;
		virtual TypeInfo GetDeleterTypeInfo() const = 0;
	};

	template<typename T, typename DeleterT>
	class SharedControlBlock : public ISharedControlBlock
	{
	public:
		using PtrTraits = SharedPtrTraits<T>;
		using PointerType = typename PtrTraits::PointerType;

		SharedControlBlock(PointerType pointer, DeleterT deleter)
			: mResourcePointer(pointer)
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
			if (!ResourceIsAlive())
			{
				return;
			}

			mStrongRefs.fetch_add(1);
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
				DeleteResource();
			}
		}

		bool HasNoWeakRefs() const override
		{
			return mWeakRefs.load() == 0;
		}

		void IncrementWeakRefs() override
		{
			mWeakRefs.fetch_add(1);
		} 

		void DecrementWeakRefs() override
		{
			mWeakRefs.fetch_sub(1);
		} 

		bool ResourceIsAlive() const override
		{
			return bool(mResourcePointer);
		}

		TypeInfo GetResourceTypeInfo() const override
		{
			return TypeInfo::Get<T>();
		}

		TypeInfo GetDeleterTypeInfo() const override
		{
			return TypeInfo::Get<DeleterT>();
		}

	private:
		void DeleteResource()
		{
			mDeleter(std::exchange(mResourcePointer, nullptr));
		}

		PointerType mResourcePointer = nullptr;
		DeleterT mDeleter;
	
		std::atomic_uint32_t mStrongRefs;
		std::atomic_uint32_t mWeakRefs;
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
		using OtherType = typename Traits::template OtherType<OtherT>;

		template<typename OtherT>
		friend class SharedPtr;

		SharedPtr() = default;

		SharedPtr(std::nullptr_t)
			: SharedPtr()
		{
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		explicit SharedPtr(OtherT* pointer)
		{
			UndertakeOwnership(pointer, DefaultDeleter<OtherType<OtherT>>());
		}

		template<
			typename OtherT,
			typename DeleterT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		explicit SharedPtr(OtherT* pointer, DeleterT deleter)
		{
			UndertakeOwnership(pointer, deleter);
		}

		SharedPtr(const SharedPtr& other)
			: mResourcePointer(other.mResourcePointer)
			, mControlBlock(other.mControlBlock)
		{
			ShareOwnership();
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		SharedPtr(const SharedPtr<OtherT>& other)
			: mResourcePointer(other.mResourcePointer)
			, mControlBlock(other.mControlBlock)
		{
			ShareOwnership();
		}

		SharedPtr(SharedPtr&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mControlBlock(std::exchange(other.mControlBlock, nullptr))
		{
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		SharedPtr(SharedPtr<OtherT>&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mControlBlock(std::exchange(other.mControlBlock, nullptr))
		{
		}

		~SharedPtr()
		{
			GiveUpOwnership();
		}

		SharedPtr& operator=(const SharedPtr& other)
		{
			if (this == &other)
			{
				return *this;
			}

			GiveUpOwnership();
			mResourcePointer = other.mResourcePointer;
			mControlBlock = other.mControlBlock;
			ShareOwnership();

			return *this;
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		SharedPtr& operator=(const SharedPtr<OtherT>& other)
		{
			GiveUpOwnership();
			mResourcePointer = other.mResourcePointer;
			mControlBlock = other.mControlBlock;
			ShareOwnership();

			return *this;
		}

		SharedPtr& operator=(SharedPtr&& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			GiveUpOwnership();
			mResourcePointer = std::exchange(other.mResourcePointer, nullptr);
			mControlBlock = std::exchange(other.mControlBlock, nullptr);

			return *this;
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		SharedPtr& operator=(SharedPtr<OtherT>&& other) noexcept
		{
			GiveUpOwnership();
			mResourcePointer = std::exchange(other.mResourcePointer, nullptr);
			mControlBlock = std::exchange(other.mControlBlock, nullptr);

			return *this;
		}

		void Reset()
		{
			GiveUpOwnership();
		}

		void Reset(std::nullptr_t)
		{
			Reset();
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		void Reset(OtherT* pointer)
		{
			UndertakeOwnership(pointer, DefaultDeleter<OtherType<OtherT>>());
		}

		template<
			typename OtherT,
			typename DeleterT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		void Reset(OtherT* pointer, DeleterT deleter)
		{
			UndertakeOwnership(pointer, deleter);
		}

		void Swap(SharedPtr& other)
		{
			std::swap(mResourcePointer, other.mResourcePointer);
			std::swap(mControlBlock, other.mControlBlock);
		}

		PointerType Get() const
		{
			return mResourcePointer;
		}

		TypeInfo GetResourceTypeInfo() const
		{
			return mControlBlock
				? mControlBlock->GetResourceTypeInfo()
				: TypeInfo();
		}

		template<typename OtherT>
		SharedPtr<OtherT> AsUnsafe() const
		{
			SharedPtr<OtherT> other;

			other.mResourcePointer = reinterpret_cast<
				typename decltype(other)::PointerType
			>(mResourcePointer);
			other.mControlBlock = mControlBlock;
			other.ShareOwnership();

			return other;
		}

		template<typename OtherT>
		SharedPtr<OtherT> As() const
		{
			if (TypeInfo::Get<OtherT>() == GetResourceTypeInfo())
			{
				return AsUnsafe<OtherT>();
			}
			return nullptr;
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
		
		std::uint32_t UseCount() const
		{
			return mControlBlock
				? mControlBlock->GetStrongRefs()
				: 0;
		}

		explicit operator bool() const
		{
			return bool(Get());
		}

	private:
		template<typename OtherT, typename DeleterT>
		void UndertakeOwnership(OtherT* pointer, DeleterT deleter)
		{
			GiveUpOwnership();

			mResourcePointer = pointer;
			mControlBlock = new Internal::ReferencingControlBlock<
				OtherType<OtherT>,
				DeleterT
			>(
				pointer,
				deleter
			);
		}

		void ShareOwnership()
		{
			if (mControlBlock)
			{
				mControlBlock->IncrementStrongRefs();
			}
		}

		void GiveUpOwnership()
		{
			mResourcePointer = nullptr;
			if (mControlBlock)
			{
				mControlBlock->DecrementStrongRefs();
				DeleteControlBlock();
			}
		}

		void DeleteControlBlock()
		{
			if (!mControlBlock->ResourceIsAlive() && mControlBlock->HasNoWeakRefs())
			{
				delete mControlBlock;
				mControlBlock = nullptr;
			}
		}

		Internal::IControlBlock* mControlBlock = nullptr;
		PointerType mResourcePointer = nullptr;
	};

	template<
		typename T,
		typename... ArgsT
	>
	std::enable_if_t<!std::is_array_v<T>, SharedPtr<T>>
		MakeShared(ArgsT&&... args)
	{
		return SharedPtr<T>(new T{ std::forward<ArgsT>(args)... });
	}

	template<typename T>
	std::enable_if_t<std::is_array_v<T>, SharedPtr<T>>
	MakeShared(std::size_t size)
	{
		return SharedPtr<T>(new std::remove_extent_t<T>[size]{});
	}
}
