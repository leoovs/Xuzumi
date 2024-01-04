#pragma once

#include <Xuzumi/Xuzumi.hpp>

class App
{
public:
	App();

	void Run();
	void Update();
	void Quit();

private:
	bool OnFrameClose(const Xuzumi::WindowFrameClosedEvent& event);
	bool OnFrameResize(const Xuzumi::WindowFrameResizedEvent& event);
	bool OnKeyDown(const Xuzumi::KeyDownEvent& event);
	bool OnKeyUp(const Xuzumi::KeyUpEvent& event);

	bool mRunning = false;
	Xuzumi::SharedPtr<Xuzumi::PlatformService> mPlatform;
	Xuzumi::SharedPtr<Xuzumi::WindowFrame> mFrame;
	Xuzumi::EventBus mEvents;
	Xuzumi::EventSubscriber mEventSubscriber;
};
