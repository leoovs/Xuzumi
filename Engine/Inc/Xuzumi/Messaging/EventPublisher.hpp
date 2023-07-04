#ifndef XUZUMI_MESSAGING_EVENTPUBLISHER_HPP_
#define XUZUMI_MESSAGING_EVENTPUBLISHER_HPP_

#include "Xuzumi/Core/ObserverPtr.hpp"
#include "Xuzumi/Messaging/EventBus.hpp"

namespace Xuzumi
{
	class EventPublisher
	{
	public:
		void Connect(ObserverPtr<EventBus> bus);

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

#endif // XUZUMI_MESSAGING_EVENTPUBLISHER_HPP_
