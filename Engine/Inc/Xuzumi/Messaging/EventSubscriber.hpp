/**
 * @file Xuzumi/Messaging/EventSubscriber.hpp 
 * 
 * @brief Defines the `EventSubscriber` helper type.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Memory/ObserverPtr.hpp"
#include "Xuzumi/Messaging/EventBus.hpp"
#include "Xuzumi/Debug/DebugSession.hpp"

namespace Xuzumi
{
	class EventSubscriber;
}

namespace Xuzumi::Internal
{
	template<typename ClassT>
	class EventMethodSubscriptionForwarder
	{
	public:
		explicit EventMethodSubscriptionForwarder(
			EventSubscriber& subscriber,
			ClassT* classInstance
		)
			: mSubscriber(&subscriber)
			, mClassInstance(classInstance)
		{
		}

		template<typename EventT>
		EventMethodSubscriptionForwarder& Method(bool(ClassT::*method)(const EventT&));

		EventSubscriber& EndThis()
		{
			return *mSubscriber;
		}

	private:
		EventSubscriber* mSubscriber = nullptr;
		ClassT* mClassInstance = nullptr;
	};
}

namespace Xuzumi
{
	/**
	 * @brief Builder for managing multiple event subscriptions.
	 */
	class EventSubscriber
	{
	public:
		/**
		 * @brief Destructor.
		 * 
		 * Internally calls `Unsubscribe`.
		 */
		~EventSubscriber();

		/**
		 * @brief Associate `*this` with @p bus.
		 * 
		 * @param bus An event bus.
		 *
		 * @return The `*this` reference.
		 */
		EventSubscriber& Subscribe(ObserverPtr<EventBus> bus);

		/**
		 * @brief Unsubscribes from all subscriptions and dissociates with the
		 * current event bus.
		 */
		void Unsubscribe();

		/**
		 * @brief Add a function as @p EventT handler.
		 * 
		 * In debug mode, raises an assertion failure if no event bus is associated
		 * with `*this`.
		 *
		 * @tparam EventT The type of the event to handle.
		 *
		 * @param function The function pointer.
		 */
		template<typename EventT>
		EventSubscriber& Function(bool(*function)(const EventT&))
		{
			XZ_ASSERT(mBus, "Could not subscribe function: EventBus instance not set");
		
			EventSubscription subscription = mBus->Subscribe(
				EventHandler<EventT>(function)
			);

			AddSubscription(subscription);

			return *this;
		}

		/**
		 * @brief Add a functor as @p EventT handler.

		 * In debug mode, raises an assertion failure if no event bus is associated
		 * with `*this`.
		 *
		 * @tparam EventT The type of the event to handle.
		 * @tparam FunctorT The type of the functor.
		 * 
		 * @param functor The event handler.
		 */
		template<typename EventT, typename FunctorT>
		EventSubscriber& Functor(FunctorT functor)
		{
			XZ_ASSERT(mBus, "Could not subscribe functor: EventBus instance not set");

			EventSubscription subscription = mBus->Subscribe(
				EventHandler<EventT>(functor)
			);

			AddSubscription(subscription);

			return *this;
		}

		/**
		 * @brief Yields subscription to an internal entity that supports method
		 * subscription.
		 *
		 * In debug mode, raises an assertion failure if no event bus is associated
		 * with `*this`.

		 * Internal entity has the following methods:
		 * - `Method<EventT>()` - Adds method as an @p EventT handler.
		 * - `EndThis()` - Yields back the method subscription.
		 * 
		 * @tparam ClassT The type of the class whose method pointers will be used.
		 *
		 * @param classInstance A pointer to an instance of the @p ClassT that will
		 * be passed to a method, when an event occurs.
		 */
		template<typename ClassT>
		Internal::EventMethodSubscriptionForwarder<ClassT>
		BeginThis(ClassT* classInstance)
		{
			XZ_ASSERT(
				mBus,
				"Could not begin method subscription:"
				" EventBus instance not set"
			);

			return Internal::EventMethodSubscriptionForwarder(
				*this,
				classInstance
			);
		}

		/**
		 * @brief Retrieves the associated event bus.
		 *
		 * @return A pointer to the associated event bus.
		 */
		ObserverPtr<EventBus> GetBus() const;

		/**
		 * @brief Add custom subscription handle.
		 *
		 * @param subscription Custom subscription handle. 
		 */
		void AddSubscription(EventSubscription subscription);

	private:
		ObserverPtr<EventBus> mBus = nullptr;
		std::vector<EventSubscription> mSubscriptions;
	};
}

namespace Xuzumi::Internal
{
	template<typename ClassT>
	template<typename EventT>
	EventMethodSubscriptionForwarder<ClassT>&
	EventMethodSubscriptionForwarder<ClassT>::Method(
		bool(ClassT::*method)(const EventT&)
	)
	{
		EventSubscription subscription = mSubscriber->GetBus()->Subscribe(
			EventHandler<EventT>(
				mClassInstance,
				method
			)
		);

		mSubscriber->AddSubscription(subscription);

		return *this;
	}
}
