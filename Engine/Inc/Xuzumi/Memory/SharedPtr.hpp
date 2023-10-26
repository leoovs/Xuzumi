/**
 * @file Xuzumi/Memory/SharedPtr.hpp
 * 
 * @brief Defines the owning `SharedPtr` smart pointer.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Debug/DebugSession.hpp"
#include "Xuzumi/TypeMeta/TypeInfo.hpp"
#include "Xuzumi/Memory/Deleter.hpp"
#include "Xuzumi/Memory/PointerBase.hpp"
#include "Xuzumi/Memory/ControlBlock.hpp"
#include "Xuzumi/Memory/Referencer.hpp"
#include "Xuzumi/Core/Templates/IsCompatible.hpp"

namespace Xuzumi
{
	/**
	 * @brief Smart pointer with shared ownership semantics.
	 *
	 * The template parameter @p T represents the formal type known at compile
	 * time, which may differ from the actual (true) resource type stored within
	 * the `SharedPtr` internal state. To ensure type-safety, consider using
	 * type introspection methods like `Holds<T>` and `As<T>`. However, note that
	 * these methods imply a slight runtime overhead.
	 *
	 * @tparam T The formal resource type known at compile time.
	 */
	template<typename T>
	class SharedPtr
	{
	private:
		using Traits = Internal::PointerTraits<T>;

		template<typename OtherT>
		using OtherType = typename Traits::template OtherType<OtherT>;

		template<typename OtherT>
		friend class SharedPtr;

		template<typename OtherT>
		friend class WeakPtr;

	public:
		/**
		 * @typedef PointerType
		 * 
		 * @brief Type alias for a resource pointer type.
		 */
		using PointerType = typename Traits::PointerType;

		/**
		 * @typedef ReferenceType
		 *
		 * @brief Type alias for a resource reference type.
		 */
		using ReferenceType = typename Traits::ReferenceType;

		/**
		 * @brief Default constructor.
		 * 
		 * Constructs an empty `SharedPtr` with no managed object
		 */
		SharedPtr() = default;

		/**
		 * @brief Constructs an empty `SharedPtr` with explicit `nullptr` parameter. 
		 */
		SharedPtr(std::nullptr_t)
			: SharedPtr()
		{
		}

		/**
		 * @brief Constructs a `SharedPtr` with @p pointer as the pointer to the
		 * managed object.
		 *
		 * This constructor participates in overload resolution only if @p OtherT is
		 * compatible with @p T.
		 * 
		 * @tparam OtherT The type of the managed object.
		 *
		 * @param pointer The pointer to the managed object.
		 * 
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		explicit SharedPtr(OtherT* pointer)
		{
			UndertakeOwnership(pointer, DefaultDeleter<OtherType<OtherT>>());
		}

		/**
		 * @brief Constructs a `SharedPtr` with @p pointer as the pointer to the
		 * managed object and @p deleter as the custom deleter.
		 * 
		 * Custom deleter is required to implement the `operator()(OtherT*)`
		 * operator.
		 * This constructor participates in overload resolution only if @p OtherT is
		 * compatible with @p T.
		 * 
		 * @tparam OtherT The type of the managed object.
		 * @tparam DeleterT The type of the custom deleter.
		 * 
		 * @param pointer The pointer to the managed object.
		 * @param deleter The custom deleter.
		 * 
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
		template<
			typename OtherT,
			typename DeleterT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		explicit SharedPtr(OtherT* pointer, DeleterT deleter)
		{
			UndertakeOwnership(pointer, deleter);
		}

		/**
		 * @brief Copy-constructs a `SharedPtr` which shares ownership of the object
		 * managed by @p other.
		 * 
		 * If @p other does not manage an object, `*this` does not manage an object
		 * either.
		 * 
		 * @param other A `SharedPtr` object.
		 */
		SharedPtr(const SharedPtr& other)
			: mResourcePointer(other.mResourcePointer)
			, mReferencer(other.mReferencer)
		{
		}

		/**
		 * @brief Copy-constructs a `SharedPtr` that shares ownership of the object
		 * managed by @p other.
		 * 
		 * If @p other manages no object, `*this` manages no object either.
		 * This constructor participates in overload resolution only if @p OtherT is
		 * compatible with @p T.
		 *
		 * @tparam OtherT The formal type of the object managed by @p other.
		 * 
		 * @param other A `SharedPtr` object.
		 * 
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		SharedPtr(const SharedPtr<OtherT>& other)
			: mResourcePointer(other.mResourcePointer)
			, mReferencer(other.mReferencer)
		{
		}

		/**
		 * @brief Move-constructs a `SharedPtr` from @p other.
		 * 
		 * After the construction `*this` contains a copy of the pointer from @p
		 * other and a copy of the pointer to the state of @p other. If @p other is
		 * empty, the stored pointer is `nullptr`.
		 *
		 * @param other The `SharedPtr` object to be moved into `*this`.
		 */
		SharedPtr(SharedPtr&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mReferencer(std::move(other.mReferencer))
		{
		}

		/**
		 * @brief Move-constructs a `SharedPtr` from @p other.
		 * 
		 * After the construction `*this` contains a copy of the pointer from @p
		 * other and a copy of the pointer to the state of @p other. If @p other is
		 * empty, the stored pointer is `nullptr`.
		 * This constructor participates in overload resolution only if @p OtherT is
		 * compatible with @p T.
		 * 
		 * @tparam OtherT The formal type of the object managed by @p other.
		 * 
		 * @param other The `SharedPtr` object to be moved into `*this`.
		 *
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		SharedPtr(SharedPtr<OtherT>&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mReferencer(std::move(other.mReferencer))
		{
		}

		/**
		 * @brief Copy-assigns the `SharedPtr`.
		 * 
		 * Replaces the managed object with one managed by @p other and shares
		 * the ownership. Self-assignment is valid and does nothing.
		 * 
		 * @param other The `SharedPtr` object to share the ownership with.
		 * 
		 * @return `*this`.
		 */
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

		/**
		 * @brief Copy-assigns the `SharedPtr`.
		 * 
		 * Replaces the managed object with one managed by @p other and shares
		 * the ownership.
		 * This copy-assignment operator participates in overload resolution only if
		 * @p OtherT is compatible with @p T.
		 *
		 * @tparam OtherT The formal type of the object managed by @p other.
		 *
		 * @param other The `SharedPtr` object to share the ownership with.
		 * 
		 * @return `*this`.
		 * 
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
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

		/**
		 * @brief Move-assigns the `SharedPtr`.
		 * 
		 * After the assignment `*this` manages the same object that the @p other
		 * has, the @p other becomes empty. Self assignment is valid and does
		 * nothing.
		 * 
		 * @param other The `SharedPtr` object to take the ownership from.
		 * 
		 * @return `*this`.
		 */
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

		/**
		 * @brief Move-assigns the `SharedPtr`.
		 * 
		 * After the assignment `*this` manages the same object that the @p other 
		 * has, the @p other becomes empty.
		 * This move-assignment operator participates in overload resolution only if
		 * @p OtherT is compatible with @p T.
		 * 
		 * @tparam OtherT The formal type of the object managed by @p other.
		 * 
		 * @param other The `SharedPtr` object to take the ownership from.
		 * 
		 * @return `*this`.
		 * 
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
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

		/**
		 * @brief Explicit termination of the ownership.
		 * 
		 * The `*this` stops sharing the ownership of the managed object and becomes
		 * empty. If `*this` is the only strong reference, the managed object is
		 * destroyed.
		 */
		void Reset()
		{
			mReferencer.Reset();	
		}

		/**
		 * @brief Explicit termination of the ownership.
		 * 
		 * The `*this` stops sharing the ownership of the managed object and becomes 
		 * empty. If `*this` is the only strong reference, the managed object is
		 * destroyed.
		 * 
		 * @param _unnamed_ `nullptr`
		 */
		void Reset(std::nullptr_t)
		{
			Reset();
		}

		/**
		 * @brief Replaces the managed object.
		 * 
		 * Replaces the managed object with an object pointed by @p pointer.
		 * This method participates in overload resolution only if @p OtherT is
		 * compatible with @p T.
		 * 
		 * @tparam OtherT The formal type of the object pointed by @p pointer.
		 * 
		 * @param pointer The pointer to the object to be managed by `*this`.
		 * 
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		void Reset(OtherT* pointer)
		{
			UndertakeOwnership(pointer, DefaultDeleter<OtherType<OtherT>>());
		}

		/**
		 * @brief Replaces the managed object.
		 * 
		 * Replaces the managed object with an object pointed by @p pointer and
		 * applies a custom @p deleter. 
		 * This method participates in overload resolution only if @p OtherT is
		 * compatible with @p T.
		 * 
		 * @tparam OtherT The formal type of the object pointed by @p pointer.
		 * @tparam DeleterT The type of the @p deleter.	
		 * 
		 * @param pointer The pointer to the object to be managed by `*this`.
		 * @param deleter The custom deleter. 
		 */
		template<
			typename OtherT,
			typename DeleterT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		void Reset(OtherT* pointer, DeleterT deleter)
		{
			UndertakeOwnership(pointer, deleter);
		}

		/**
		 * @brief Exchanges the contents of `*this` and @p other. 
		 * 
		 * @param other The `SharedPtr` to exchange contents with.
		 */
		void Swap(SharedPtr& other)
		{
			std::swap(mResourcePointer, other.mResourcePointer);
			std::swap(mReferencer, other.mReferencer);
		}

		/**
		 * @brief Returns the stored pointer. 
		 * 
		 * @return The stored pointer.
		 */
		PointerType Get() const
		{
			return mResourcePointer;
		}

		/**
		 * @brief Returns the managed object type information.
		 * 
		 * @return The managed object type information.
		 */
		TypeInfo GetResourceTypeInfo() const
		{
			return mReferencer.GetResourceTypeInfo();
		}

		/**
		 * @brief Reinterprets the `SharedPtr<T>` to `SharedPtr<OtherT>` without
		 * any additional runtime type check.
		 * 
		 * A new instance of the `SharedPtr<OtherT>` points to the same object in
		 * memory and shares the ownership.
		 * Since no type checking is performed, it is basically a plain
		 * `reinterpret_cast`. Accessing such object might lead to an undefined
		 * behavior. Consider using a type-safe analogue `As`.
		 * 
		 * @tparam OtherT The type of the object to cast to.
		 * 
		 * @return An instance of `SharedPtr<OtherT>`.
		 * 
		 * @see SharedPtr::As
		 */
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

		/**
		 * @brief Converts the `SharedPtr<T>` to `SharedPtr<OtherT>` in a type-safe
		 * way performing additional runtime type check.
		 * 
		 * A new instance of the `SharedPtr<OtherT>` points to the same object in
		 * memory and shares the ownership.
		 * 
		 * @tparam OtherT The type of the object to convert to.
		 * 
		 * @retval SharedPtr<OtherT> Conversion is successful and type-safe.
		 * @retval nullptr Conversion is not type-safe and
		 * therefore cannot be performed.
		 */
		template<typename OtherT>
		SharedPtr<OtherT> As() const
		{
			if (Holds<OtherT>())
			{
				return AsUnsafe<OtherT>();
			}
			return nullptr;
		}

		/**
		 * @brief Checks whether the `*this` holds an object of the specified type.
		 * 
		 * @tparam OtherT The type to check against.
		 * 
		 * @retval true The @p OtherT and the managed object type match.
		 * @retval false The @p OtherT and the managed object type mismatch.
		 */
		template<typename OtherT>
		bool Holds() const
		{
			return TypeInfo::Get<OtherT>() == GetResourceTypeInfo();
		}

		/**
		 * @brief Dereferences the stored pointer.
		 * 
		 * In debug mode, the runtime performs and assertion to see whether the
		 * `*this` is empty.
		 *  
		 * @return A reference to the managed object.
		 */
		ReferenceType operator*() const
		{
			XZ_ASSERT(*this, "Trying to dereference a nullptr");
			return *Get();
		}

		/**
		 * @brief Dereferences the stored pointer.
		 * 
		 * In debug mode, the runtime performs an assertion to ensure that the
		 * `*this` is not empty.
		 * 
		 * @return A pointer to the managed object.
		 */
		PointerType operator->() const
		{
			XZ_ASSERT(*this, "Trying to dereference a nullptr");
			return Get();
		}

		/**
		 * @brief Index to the array pointed to by the stored pointer.
		 * 
		 * In debug mode, the runtime performs an assertion to ensure that the
		 * `*this` is not empty. 
		 * 
		 * @param index The array index.
		 * 
		 * @return A reference to the `index`-th element in the array.
		 */
		ReferenceType operator[](std::ptrdiff_t index) const
		{
			XZ_ASSERT(*this, "Trying to dereference a nullptr");
			return Get()[index];
		}
		
		/**
		 * @brief Retrieve the number of different `SharedPtr` instances that share 
		 * the ownership.
		 *
		 * @return The number of different `SharedPtr` instances managing the
		 * current object. If the `*this` is empty, `0` is returned.
		 */
		std::uint32_t UseCount() const
		{
			return mReferencer.UseCount();
		}

		/**
		 * @brief Checks if the `*this` is not empty.
		 * 
		 * @retval true The stored pointer is not `nullptr`.
		 * @retval false The stored pointer is `nullptr`. The `*this` is empty. 
		 */
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

	/**
	 * @brief Constructs an object of type @p T and wraps it into `SharedPtr`.
	 *
	 * The object is constructed as if by the expression
	 * `new T{ std::forward<ArgsT>(args)... }`
	 * 
	 * @tparam T The type of the object to be constructed.
	 * 
	 * @param args The list of arguments with which an instance of @p T will be
	 * constructed
	 * 
	 * @return The `SharedPtr` instance managing the constructed object of type
	 * @p T.
	 */
	template<
		typename T,
		typename... ArgsT
	>
	std::enable_if_t<!std::is_array_v<T>, SharedPtr<T>>
		MakeShared(ArgsT&&... args)
	{
		return SharedPtr<T>(new T{ std::forward<ArgsT>(args)... });
	}

	/**
	 * @brief Constructs an array of objects of type @p T and wraps it into
	 * `SharedPtr`.
	 * 
	 * The objects in the array are value-initialized, i.e. by the `new T[size]{}`
	 * expression.
	 *
	 * @tparam T The array type.
	 * 
	 * @param size The amount of objects in the array.
	 * 
	 * @return The `SharedPtr` instance managing the constructed array of objects.
	 */
	template<typename T>
	std::enable_if_t<std::is_array_v<T>, SharedPtr<T>>
	MakeShared(std::size_t size)
	{
		return SharedPtr<T>(new std::remove_extent_t<T>[size]{});
	}
}
