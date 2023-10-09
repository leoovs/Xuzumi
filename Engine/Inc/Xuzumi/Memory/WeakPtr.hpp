/**
 * @file Xuzumi/Memory/WeakPtr.hpp
 * 
 * @brief Defines the non-owning `WeakPtr` smart pointer.
 */

#pragma once

#include "Xuzumi/Memory/SharedPtr.hpp"
#include "Xuzumi/Memory/PointerBase.hpp"
#include "Xuzumi/Memory/Referencer.hpp"
#include "Xuzumi/Core/Templates/IsCompatible.hpp"

namespace Xuzumi
{
	/**
	 * @brief Smart pointer with weak reference to the resource.
	 * 
	 * The template parameter @p T represents the formal type known at compile
	 * time, which may differ from the actual (true) resource type that the
	 * `WeakPtr` internal state references.
	 * 
	 * @tparam T The formal resource type known at compile time.
	 */
	template<typename T>
	class WeakPtr
	{
	public:
		using Traits = Internal::PointerTraits<T>; // TODO: make this private.j

		/**
		 * @typedef PointerType
		 * 
		 * @brief Type alias for a resource pointer type.
		 */
		using PointerType = typename Traits::PointerType;

		template<typename OtherT>
		friend class WeakPtr;

		/**
		 * @brief Default constructor.
		 * 
		 * Constructs an empty `WeakPtr` that does not reference an object.
		 */
		WeakPtr() = default;

		/**
		 * @brief Constructs an empty `WeakPtr` with explicit `nullptr` parameter.
		 */
		WeakPtr(std::nullptr_t)
			: WeakPtr()
		{
		}

		/**
		 * @brief Constructs a `WeakPtr` that potentially holds a weak reference to
		 * an object managed by @p ptr.
		 * 
		 * This constructor participates in overload resolution only if @p OtherT is
		 * compatible with @p T.
		 * 
		 * @tparam The formal type of the referenced object managed by @p ptr.
		 * 
		 * @param ptr The `SharedPtr` that contains an object to be referenced.
		 *
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
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

		/**
		 * @brief Copy-constructs a `WeakPtr` that potentially holds a weak
		 * reference to an object referenced by @p other.
		 * 
		 * If @p other does not reference an object, `*this` does not reference an
		 * object either.
		 * 
		 * @param other The `WeakPtr` holding a reference to an object to be viewed
		 * by `*this`.
		 */
		WeakPtr(const WeakPtr& other)
			: mResourcePointer(other.mResourcePointer)
			, mReferencer(other.mReferencer)
		{
		}

		/**
		 * @brief Copy-constructs a `WeakPtr` that potentially holds a weak
		 * reference to an object referenced by @p other.
		 * 
		 * If @p other does not reference an object, `*this` does not reference an
		 * object either.
		 * This constructor participates in overload resolution only if @p OtherT is
		 * compatible with @p T.
		 * 
		 * @tparam OtherT The formal type of the referenced object.
		 * 
		 * @param other The `WeakPtr` holding a reference to an object to be viewed
		 * by `*this`.
		 * 
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		WeakPtr(const WeakPtr<OtherT>& other)
			: mResourcePointer(other.mResourcePointer)
			, mReferencer(other.mReferencer)	
		{
		}

		/**
		 * @brief Move-constructs a `WeakPtr` that references the same object as
		 * @p other.
		 * 
		 * After the construction, `*this` references an object that @p other has
		 * referenced before the move-construction.
		 * 
		 * @param other The `WeakPtr` to be moved into `*this`.
		 */
		WeakPtr(WeakPtr&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mReferencer(std::move(other.mReferencer))
		{
		}

		/**
		 * @brief Move-constructs a `WeakPtr` that references the same object as
		 * @p other.
		 * 
		 * After the construction, `*this` references an object that @p other has
		 * referenced before the move-construction.
		 * This move-constructor participates in overload resolution only if
		 * @p OtherT is compatible with @p T.
		 *
		 * @tparam OtherT The formal type of the referenced object.
		 *
		 * @param other The `WeakPtr` to be moved into `*this`.
		 * 
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		WeakPtr(WeakPtr<OtherT>&& other) noexcept
			: mResourcePointer(std::exchange(other.mResourcePointer, nullptr))
			, mReferencer(std::move(other.mReferencer))
		{
		}

		/**
		 * @brief Copy-assign the `WeakPtr`.
		 *
		 * Replaces the referenced object with one referenced by @p other.
		 * If @p other does not reference an object, `*this` does not reference an
		 * object either.
		 * 
		 * @param other The `WeakPtr` that possibly references an object.
		 */
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

		/**
		 * @brief Copy-assign the `WeakPtr`.
		 * 
		 * Replaces the referenced object with one referenced by @p other.
		 * If @p other does not reference an object, `*this` does not reference an
		 * object either.
		 * This copy-assignment operator participates in overload resolution only if
		 * @p OtherT is compatible with @p T.
		 * 
		 * @tparam OtherT The formal type of the referenced object.
		 * 
		 * @param other The `WeakPtr` that possibly references an object.
		 *
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
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

		/**
		 * @brief Assigns the `WeakPtr` from `SharedPtr`.
		 * 
		 * Replaces referenced object with one managed by @p ptr.
		 * This assignment operator participates in overload resolution only if
		 * @p OtherT is compatible with @p T.
		 *
		 * @tparam The formal type of the referenced object.
		 * 
		 * @param ptr The `SharedPtr` to share an object with.
		 * 
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
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

		/**
		 * @brief Move-assigns the `WeakPtr`.
		 * 
		 * Replaces the referenced object with one referenced by @p other.
		 * After the assignment @p other does not reference the object.
		 *
		 * @param other The `WeakPtr` that possibly references an object.
		 */
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
		
		/**
		 * @brief Move-assigns the `WeakPtr`.
		 * 
		 * Replaces the referenced object with one referenced by @p other.
		 * After the assignment @p other does not reference the object.
		 * This move-assignment operator participates in overload resolution only if
		 * @p OtherT is compatible with @p T.j
		 *
		 * @tparam OtherT The formal type of the referenced object.
		 * 
		 * @param other The `WeakPtr` that possibly references an object.
		 *
		 * @see Xuzumi::IsCompatible
		 * @see Xuzumi::IsCompatibleV
		 */
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

		/**
		 * @brief Released the reference to an object.
		 * 
		 * After call `*this` is empty.
		 */
		void Reset()
		{
			mResourcePointer = nullptr;
			mReferencer.Reset();
		}

		/**
		 * @brief Exchanges the contents of `*this` and @p other.
		 * 
		 * @param other The `WeakPtr` to exchange contents with.
		 */
		void Swap(WeakPtr& other)
		{
			std::swap(mResourcePointer, other.mResourcePointer);
			std::swap(mReferencer, other.mReferencer);
		}

		/**
		 * @brief Retrieves the number of strong object references.
		 *
		 * @return The number of `SharedPtr` instances that manage the referenced
		 * object ownership, or 0 if `*this` is empty.
		 */
		std::uint32_t UseCount() const
		{
			return mReferencer.UseCount();
		}

		/**
		 * @brief Checks whether the referenced object has been deleted.
		 * 
		 * @retval true The referenced object is deleted, `*this` is empty.
		 * @retval false The referenced object has not been deleted and is valid. 
		 */
		bool Expired() const
		{
			return UseCount() == 0;
		}

		/**
		 * @brief Creates a `SharedPtr` that shares the ownership of the referenced
		 * object.
		 * 
		 * If `*this` is empty or expired, the returned `SharedPtr` instance is
		 * empty.
		 * 
		 * @return The `SharedPtr` instance that manages the referenced object.
		 */
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

		/**
		 * @brief Creates a `SharedPtr` that manages the referenced object.
		 * 
		 * Equivalent to `Lock()`.
		 * 
		 * @return The `SharedPtr` instance that manages the referenced object.
		 */
		explicit operator SharedPtr<T>() const
		{
			return Lock();
		}

	private:
		PointerType mResourcePointer = nullptr;
		Internal::Referencer mReferencer;
	};
}
