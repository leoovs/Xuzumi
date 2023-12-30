#include "App.hpp"

#include "Entity.hpp"

App::App()
{
	auto events = Xuzumi::ObserverPtr<Xuzumi::EventBus>(&mEvents);

	mEventSubscriber.Subscribe(events);

	mEventSubscriber
		.BeginThis(this)
			.Method(&App::OnFrameClose)
			.Method(&App::OnFrameResize)
		.EndThis();

	mPlatform = Xuzumi::CreatePlatformService();
	mPlatform->Connect(events);

	Xuzumi::WindowFrameSpecification frameSpec;
	frameSpec.Caption = "Hello world";
	frameSpec.Resizable = true;
	frameSpec.Visible = false;

	mFrame = mPlatform->CreateWindowFrame(frameSpec);
	mFrame->Show();
}

void App::Run()
{
	mRunning = true;
	while (mRunning)
	{
		Update();
	}
}

void App::Update()
{
	mPlatform->PollEvents();
	mEvents.Dispatch();
}

void App::Quit()
{
	mRunning = false;
}

bool App::OnFrameClose(const Xuzumi::WindowFrameClosedEvent& event)
{
	if (mFrame.Get() == event.ClosedFrame.Get())
	{
		Quit();
	}

	return true;
}

bool App::OnFrameResize(const Xuzumi::WindowFrameResizedEvent& event)
{
	XZ_LOG(Info, "New size: %u, %u", event.Width, event.Height);
	return true;
}
