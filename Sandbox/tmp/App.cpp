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
			.Method(&App::OnKeyDown)
			.Method(&App::OnKeyUp)
		.EndThis();

	mPlatform = Xuzumi::CreatePlatformService();
	mPlatform->Connect(events);

	Xuzumi::WindowFrameSpecification frameSpec;
	frameSpec.Caption = "Hello world";
	frameSpec.Resizable = true;
	frameSpec.Visible = true;

	mFrame = mPlatform->CreateWindowFrame(frameSpec);
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

bool App::OnKeyDown(const Xuzumi::KeyDownEvent& event)
{
	auto inputDevice = mPlatform->GetInputDevice();

	XZ_LOG(Info, "Key down: %d", event.KeyDown);
	
	return true;
}

bool App::OnKeyUp(const Xuzumi::KeyUpEvent& event)
{
	XZ_LOG(Info, "Key up: %d", event.KeyUp);

	return true;
}
