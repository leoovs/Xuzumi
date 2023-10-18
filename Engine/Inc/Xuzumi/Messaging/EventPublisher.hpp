/**
 * @file Xuzumi/Messaging/EventPublisher.hpp 
 *
 * @brief Defines the `EventPublisher` helper type.
 */

#pragma once

#include "Xuzumi/Memory/ObserverPtr.hpp"
#include "Xuzumi/Messaging/EventBus.hpp"

namespace Xuzumi
{
	/**
	 * @brief Helper class for publishing an event.
	 */
	class EventPublisher
	{
	public:
		/**
		 * @brief Associate `*this` with @p bus.
		 * 
		 * @param bus A pointer to an event bus.
		 */
		void Connect(ObserverPtr<EventBus> bus);

		/**
		 * @brief Publish event @p EventT if an event bus is connected.
		 *
		 * @tparam EventT The type of the event to publish.
		 * @tparam ArgsT The type list of arguments.
		 * 
		 * @param args The arguments with which an instance of @p EventT will be
		 * constructed.
		 */
		template<typename EventT, typename... ArgsT>
		void Publish(ArgsT&&... args)
		{
			if (mBus)
			{
				mBus->Publish<EventT>(std::forward<ArgsT>(args)...);
			}
		}

	private:
		ObserverPtr<EventBus> mBus;
	};
}
