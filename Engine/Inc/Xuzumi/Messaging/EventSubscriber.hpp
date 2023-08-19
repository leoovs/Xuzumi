#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Memory/ObserverPtr.hpp"
#include "Xuzumi/Messaging/EventBus.hpp"

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
	class EventSubscriber
	{
	public:
		~EventSubscriber();

		EventSubscriber& Subscribe(EventBus& bus);
		void Unsubscribe();

		template<typename EventT>
		EventSubscriber& Function(bool(*function)(const EventT&))
		{
			// TODO: check if bus is set.
		
			EventSubscription subscription = mBus->Subscribe(
				Internal::EventHandler<EventT>(function)
			);

			AddSubscription(subscription);

			return *this;
		}

		template<typename EventT, typename FunctorT>
		EventSubscriber& Functor(FunctorT functor)
		{
			EventSubscription subscriptoin = mBus->Subscribe(
				Internal::EventHandler<EventT>(functor)
			);

			AddSubscription(subscriptoin);

			return *this;
		}

		template<typename ClassT>
		Internal::EventMethodSubscriptionForwarder<ClassT> BeginThis(
			ClassT* classInstance
		)
		{
			return Internal::EventMethodSubscriptionForwarder(
				*this,
				classInstance
			);
		}

		EventBus& GetBus() const;

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
		EventSubscription subscription = mSubscriber->GetBus().Subscribe(
			EventHandler<EventT>(
				mClassInstance,
				method
			)
		);

		mSubscriber->AddSubscription(subscription);

		return *this;
	}
}
