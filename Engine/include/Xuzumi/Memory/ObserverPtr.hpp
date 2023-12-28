/**
 * @file Xuzumi/Memory/ObserverPtr.hpp
 * 
 * @brief Defines non-owning `ObserverPtr` pointer wrapper.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Memory/PointerBase.hpp"
#include "Xuzumi/Core/Templates/IsCompatible.hpp"

namespace Xuzumi
{
	/**
	 * @brief Non-owning observer pointer utility type that acts as a wrapper
	 * around a raw pointer and provides a slightly more explicit interface for
	 * pointer manipulations.
	 */
	template<typename T>
	class ObserverPtr
	{
	private:
		using Traits = Internal::PointerTraits<T>;

	public:
		/**
		 * @typedef ReferenceType
		 * 
		 * @brief Type alias for a resource reference.
		 */
		using ReferenceType = typename Traits::ReferenceType;

		/**
		 * @typedef PointerType
		 * 
		 * @brief Type alias for a pointer to a resource.
		 */
		using PointerType = typename Traits::PointerType;

		/**
		 * @brief Constructs an `ObserverPtr` object that holds `nullptr`.
		 */
		ObserverPtr() = default;
		
		/**
		 * @brief Constructs an `ObserverPtr` object that holds `nullptr` with
		 * explicit `nullptr` parameter.
		 */
		ObserverPtr(std::nullptr_t)
			: ObserverPtr()
		{
		}

		/**
		 * @brief Copy-constructs an `ObserverPtr` type from other pointer of
		 * different type.
		 * 
		 * Constructor participates in the overload resolution only if the`OtherT`
		 * type is compatible with the `T` type (to be implemented).
		 * 
		 * @see Xuzumi::IsCompatible
		 */
		template<
			typename OtherT,
			typename = std::enable_if_t<IsCompatibleV<OtherT, T>>
		>
		ObserverPtr(const ObserverPtr<OtherT>& other)
			: mWatchedObject(other.Get())
		{
		}

		/**
		 * @brief Constructs an `ObserverPtr` object that stores specified pointer.
		 * 
		 * @param watchedObject A pointer to an object to store.
		 */
		explicit ObserverPtr(PointerType watchedObject)
			: mWatchedObject(watchedObject)
		{
		}

		ObserverPtr(const ObserverPtr& other) = default;
		ObserverPtr(ObserverPtr&& other) noexcept = default;

		ObserverPtr& operator=(const ObserverPtr& other) = default;
		ObserverPtr& operator=(ObserverPtr&& other) noexcept = default;

		/**
		 * @brief Stores `nullptr` and returns the previously held pointer.
		 * 
		 * @return A previously held pointer.
		 */
		PointerType Release()
		{
			return std::exchange(mWatchedObject, nullptr);
		}

		/**
		 * @brief Sets a new pointer.
		 * 
		 * @param watchedObject A new pointer to store.
		 */
		void Reset(PointerType watchedObject = nullptr)
		{
			mWatchedObject = watchedObject;
		}

		/**
		 * @brief Retrieves the held pointer.
		 *
		 * @return The held pointer.
		 */
		PointerType Get() const
		{
			return mWatchedObject;
		}

		/**
		 * @brief Checks whether an `ObserverPtr` object holds a non-null pointer.
		 * 
		 * @retval true The pointer is not `nullptr`.
		 * @retval false The pointer is `nullptr`.
		 */
		explicit operator bool() const
		{
			return bool(mWatchedObject);
		}

		/**
		 * @brief Pointer dereference operator.
		 *
		 * Dereferences the underlying pointer.
		 *
		 * @return Reference to the object that underlying pointer points to.
		 */
		ReferenceType operator*() const
		{
			return *mWatchedObject;
		}

		/**
		 * @brief Pointer member access operator.
		 * 
		 * @return The underlying pointer.
		 */
		PointerType operator->() const
		{
			return mWatchedObject;
		}

	private:
		PointerType mWatchedObject = nullptr;
	};
}
