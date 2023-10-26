/**
 * @file Xuzumi/Messaging/EventBus.hpp 
 *
 * @brief Defines the `EventBus` type.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Core/Delegate.hpp"
#include "Xuzumi/Messaging/Event.hpp"
#include "Xuzumi/IDFramework/HandleGenerator.hpp"

namespace Xuzumi
{
	namespace Internal
	{
		using EventHandlerID = HandleID;
		inline constexpr auto kInvalidEventHandlerID = EventHandlerID(
			kInvalidHandleID
		);
	}

	/**
	 * @typedef EventHandler
	 * 
	 * @brief A type alias for delegate representing an event handler.
	 *
	 * @tparam EventT The type of the event to handle
	 */
	template<typename EventT>
	using EventHandler = Delegate<bool(const EventT&)>;

	/**
	 * @brief An event subscription handle.
	 *
	 * Holds an event subscription data that binds specified delegate and internal
	 * callback entry.
	 */
	class EventSubscription
	{
	public:
		/**
		 * @brief Default constructor.
		 * 
		 * Constructs invalid event subscription handle.
		 */
		EventSubscription() = default;

		/**
		 * @brief Copy constructor.
		 */
		EventSubscription(const EventSubscription& other) = default;

		/**
		 * @brief Constructs event subscription handle with specified internal
		 * parameters.
		 * 
		 * This constructor should not be considered part of public API.
		 */
		explicit EventSubscription(TypeID eventTypeID, Internal::EventHandlerID handlerID);

		/**
		 * @brief Retrieves an event type ID that the bound handler is listening to.
		 *
		 * @return Type ID of the event that the bound handler is listening to.
		 */
		TypeID GetEventTypeID() const;

		/**
		 * @brief Internal method, retrieves implementation-defined ID.
		 */
		Internal::EventHandlerID GetHandlerID() const;

	private:
		TypeID mEventTypeID = kInvalidTypeID;
		Internal::EventHandlerID mHandlerID = Internal::kInvalidEventHandlerID;
	};
}

namespace Xuzumi::Internal
{
	class IEventDispatcher
	{
	public:
		virtual ~IEventDispatcher() = default;

		virtual void FlushEvents() = 0;
		virtual void Dispatch() = 0;
		virtual void RemoveHandler(EventHandlerID handlerID) = 0;
		virtual void RemoveAllHandlers() = 0;
	};

	template<typename EventT>
	class EventDispatcher : public IEventDispatcher
	{
	public:
		void FlushEvents() override
		{
			mEventStack.clear();
		}
		
		void Dispatch() override
		{
			for (auto it = mEventStack.rbegin(); it != mEventStack.rend();)
			{
				if (NotifyHandlers(*it))
				{
					it = std::make_reverse_iterator(
						mEventStack.erase(std::next(it).base())
					);
					continue;
				}
				it++;
			}
		}

		void RemoveHandler(EventHandlerID handlerID) override
		{
			if (handlerID < mHandlerQueue.size() 
				&& mHandlerIDGenerator.Exists(handlerID))
			{
				mHandlerQueue[handlerID].Reset();
				mHandlerIDGenerator.Destroy(handlerID);
			}
		}
	
		void RemoveAllHandlers() override
		{
			mHandlerIDGenerator.Reset();
			mHandlerQueue.clear();
		}

		template<typename... ArgsT>
		void PushEvent(ArgsT&&... args)
		{
			mEventStack.emplace_back(std::forward<ArgsT>(args)...);
		}

		EventHandlerID AddHandler(EventHandler<EventT> handler)
		{
			EventHandlerID id = mHandlerIDGenerator.Create();
		
			if (id >= mHandlerQueue.size())
			{
				mHandlerQueue.resize(id + 1);
			}
			mHandlerQueue[id] = handler;

			return id;
		}

	private:
		bool NotifyHandlers(const EventT& event)
		{
			for (auto& handler : mHandlerQueue)
			{
				if (handler && handler(event))
				{
					return true;
				}
			}
			return false;
		}

		std::deque<EventT> mEventStack;
		std::vector<EventHandler<EventT>> mHandlerQueue;
		HandleGenerator mHandlerIDGenerator;
	};
}

namespace Xuzumi
{
	/**
	 * @brief Event organization facility.
	 * 
	 * Implements a registry that stores events and their corresponding handlers
	 * and enables their interaction.
	 */
	class EventBus
	{
	public:
		/**
		 * @brief Adds event @p EventT to an event queue.
		 * 
		 * @tparam EventT The event type.
		 * @tparam ArgsT The type list of arguments.
		 * 
		 * @param args The list of arguments with which an instance of @p EventT
		 * will be constructed.
		 */
		template<typename EventT, typename... ArgsT>
		void Publish(ArgsT&&... args)
		{
			GetDispatcher<EventT>()->PushEvent(std::forward<ArgsT>(args)...);
		}

		/**
		 * @brief Registers an event handler for events of type @p EventT.
		 * 
		 * @tparam EventT The type of the event to handle.
		 *
		 * @param handler The event handler.
		 * 
		 * @return An instance of `EventSubscription`.
		 */
		template<typename EventT>
		EventSubscription Subscribe(EventHandler<EventT> handler)
		{
			TypeID eventTypeID = GetTypeID<EventT>();
			Internal::EventHandlerID handlerID = 
				GetDispatcher<EventT>()->AddHandler(handler);
		
			return EventSubscription(eventTypeID, handlerID);
		}

		/**
		 * @brief Remove an event handler associated with @p subscription.
		 *
		 * @param subscription An `EventSubscription` instance associated with
		 * handler that is to be removed.
		 */
		void Unsubscribe(EventSubscription subscription);

		/**
		 * @brief Removes all event handlers.
		 */
		void UnsubscribeAll();

		/**
		 * @brief Traverses an event queue dispatching the contained events to the
		 * registered event handlers.
		 * 
		 * If an event handler returns `true` the event is considered handled and
		 * is removed from the queue; otherwise the event persists and is passed to
		 * another handlers if there are any.
		 */
		void Dispatch();

		/**
		 * @brief Clears and event queue.
		 */
		void FlushEvents();

	private:
		template<typename EventT>
		std::shared_ptr<Internal::EventDispatcher<EventT>> GetDispatcher()
		{
			TypeID eventTypeID = GetTypeID<EventT>();
			auto dispatcherIt = mDispatchers.find(eventTypeID);

			if (mDispatchers.end() == dispatcherIt)
			{
				auto dispatcher = std::make_shared<
					Internal::EventDispatcher<EventT>
				>();

				mDispatchers[eventTypeID] = dispatcher;

				return dispatcher;
			}

			return std::static_pointer_cast<
				Internal::EventDispatcher<EventT>
			>(dispatcherIt->second);
		}

		std::unordered_map<
			TypeID,
			std::shared_ptr<Internal::IEventDispatcher>
		> mDispatchers;
	};
}
