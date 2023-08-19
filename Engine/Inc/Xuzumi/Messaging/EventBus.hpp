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
		inline constexpr EventHandlerID kInvalidEventHandlerID = EventHandlerID(
			kInvalidHandleID
		);
	}

	class EventSubscription
	{
	public:
		EventSubscription() = default;
		EventSubscription(const EventSubscription& other) = default;

		explicit EventSubscription(TypeID eventTypeID, Internal::EventHandlerID handlerID);

		TypeID GetEventTypeID() const;
		Internal::EventHandlerID GetHandlerID() const;

	private:
		TypeID mEventTypeID = kInvalidTypeID;
		Internal::EventHandlerID mHandlerID = Internal::kInvalidEventHandlerID;
	};
}

namespace Xuzumi::Internal
{
	template<typename EventT>
	using EventHandler = Delegate<bool(const EventT&)>;

	class IEventDispatcher
	{
	public:
		virtual ~IEventDispatcher() = default;

		virtual void Reset() = 0;
		virtual void Dispatch() = 0;
		virtual void RemoveHandler(EventHandlerID handlerID) = 0;
	};

	template<typename EventT>
	class EventDispatcher : public IEventDispatcher
	{
	public:
		void Reset() override
		{
			mHandlerIDGenerator.Reset();
			mHandlerQueue.clear();
		}

		void Dispatch() override
		{
			while (!mEventStack.empty())
			{
				const EventT& event = mEventStack.top();
			
				for (auto& handler : mHandlerQueue)
				{
					if (handler && handler(event))
					{
						break;
					}
				}

				mEventStack.pop();
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
	
		template<typename... ArgsT>
		void PushEvent(ArgsT&&... args)
		{
			mEventStack.emplace(std::forward<ArgsT>(args)...);
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
		std::stack<EventT> mEventStack;
		std::vector<EventHandler<EventT>> mHandlerQueue;
		HandleGenerator mHandlerIDGenerator;
	};
}

namespace Xuzumi
{
	class EventBus
	{
	public:
		template<typename EventT, typename... ArgsT>
		void Publish(ArgsT&&... args)
		{
			GetDispatcher<EventT>()->PushEvent(std::forward<ArgsT>(args)...);
		}

		template<typename EventT>
		EventSubscription Subscribe(Internal::EventHandler<EventT> handler)
		{
			TypeID eventTypeID = GetTypeID<EventT>();
			Internal::EventHandlerID handlerID = 
				GetDispatcher<EventT>()->AddHandler(handler);
		
			return EventSubscription(eventTypeID, handlerID);
		}


		void Unsubscribe(EventSubscription subscription);

		void Dispatch();

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
