#include <iostream>

#include <Xuzumi/Messaging/EventBus.hpp>
#include <Xuzumi/Messaging/EventPublisher.hpp>
#include <Xuzumi/Messaging/EventSubscriber.hpp>

struct AppShutdownEvent : public Xuzumi::Event<AppShutdownEvent> {};

class App
{
public:
	App(Xuzumi::ObserverPtr<Xuzumi::EventBus> bus)
	{
		mSubscriber.Subscribe(*bus)
			.BeginThis(this)
				.Method(&App::OnShutdown)
			.EndThis()
			.Functor<AppShutdownEvent>(
				[this](auto& event)
				{
					CustomOnShutdown();
					return true;
				}
			);
	}

	bool IsRunning() const
	{
		return mRunning;
	}

	void Quit()
	{
		mRunning = false;
	}

private:
	bool OnShutdown(const AppShutdownEvent& event)
	{
		std::cout << XZ_FUNCNAME << std::endl;
		Quit();
		return false;
	}

	void CustomOnShutdown()
	{
		std::cout << XZ_FUNCNAME << std::endl;
	}

	Xuzumi::EventSubscriber mSubscriber;
	bool mRunning = true;
};

int main()
{
	Xuzumi::EventBus events;
	Xuzumi::EventPublisher publisher;

	Xuzumi::ObserverPtr<Xuzumi::EventBus> eventsPtr(&events);

	auto app = new App(eventsPtr);
	publisher.Connect(eventsPtr);

	while (app->IsRunning())
	{
		publisher.Publish<AppShutdownEvent>();
		events.Dispatch();
	}
	
	delete app;
	app = nullptr;
}

