#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Debug/Assertion.hpp"
#include "Xuzumi/TypeMeta/TypeInfo.hpp"
#include "Xuzumi/Memory/Deleter.hpp"
#include "Xuzumi/Memory/PointerBase.hpp"
#include "Xuzumi/Memory/ControlBlock.hpp"
#include "Xuzumi/Memory/Referencer.hpp"
#include "Xuzumi/Core/Templates/IsCompatible.hpp"

namespace Xuzumi
{
	template<typename T>
	class SharedPtr
	{
	public:
		using Traits = Internal::PointerTraits<T>;

		using ValueType = typename Traits::ValueType;
		using PointerType = typename Traits::PointerType;
		using ReferenceType = typename Traits::ReferenceType;

		template<typename OtherT>
		using OtherType = typename Traits::template OtherType<OtherT>;

		template<typename OtherT>
		friend class SharedPtr;

		template<typename OtherT>
		friend class WeakPtr;

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
			, mReferencer(other.mReferencer)
		{
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		SharedPtr(const SharedPtr<OtherT>& other)
			: mResourcePointer(other.mResourcePointer)
			, mReferencer(other.mReferencer)
		{
		}

		SharedPtr(SharedPtr&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mReferencer(std::move(other.mReferencer))
		{
		}

		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		SharedPtr(SharedPtr<OtherT>&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mReferencer(std::move(other.mReferencer))
		{
		}

		SharedPtr& operator=(const SharedPtr& other)
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
		SharedPtr& operator=(const SharedPtr<OtherT>& other)
		{
			mResourcePointer = other.mResourcePointer;
			mReferencer = other.mReferencer;

			return *this;
		}

		SharedPtr& operator=(SharedPtr&& other) noexcept
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
		SharedPtr& operator=(SharedPtr<OtherT>&& other) noexcept
		{
			mResourcePointer = std::exchange(other.mResourcePointer, nullptr);
			mReferencer = std::move(other.mReferencer);

			return *this;
		}

		void Reset()
		{
			mReferencer.Reset();	
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
			std::swap(mReferencer, other.mReferencer);
		}

		PointerType Get() const
		{
			return mResourcePointer;
		}

		TypeInfo GetResourceTypeInfo() const
		{
			return mReferencer.GetResourceTypeInfo();
		}

		template<typename OtherT>
		SharedPtr<OtherT> AsUnsafe() const
		{
			SharedPtr<OtherT> other;

			other.mResourcePointer = reinterpret_cast<
				typename decltype(other)::PointerType
			>(mResourcePointer);
			other.mReferencer = mReferencer;

			return other;
		}

		template<typename OtherT>
		SharedPtr<OtherT> As() const
		{
			if (Holds<OtherT>())
			{
				return AsUnsafe<OtherT>();
			}
			return nullptr;
		}

		template<typename OtherT>
		bool Holds() const
		{
			return TypeInfo::Get<OtherT>() == GetResourceTypeInfo();
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
			return mReferencer.UseCount();
		}

		explicit operator bool() const
		{
			return bool(Get());
		}

	private:
		template<typename OtherT, typename DeleterT>
		void UndertakeOwnership(OtherT* pointer, DeleterT deleter)
		{
			mResourcePointer = pointer;
			mReferencer.Bind<
				Internal::ReferencingControlBlock<OtherType<OtherT>, DeleterT>
			>(pointer, deleter);	
		}

		Internal::Referencer mReferencer;
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
