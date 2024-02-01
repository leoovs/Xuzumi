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
	bool OnChar(const Xuzumi::CharacterInputEvent& event);

	bool mRunning = false;

	// TODO: optimally, event bus should live longer that the application itself
	// in order to avoid life-time issues, e.g input device tries to unsubscribe from
	// platform events referencing a dead event bus.

	Xuzumi::EventBus mEvents;
	Xuzumi::EventSubscriber mEventSubscriber;
	Xuzumi::SharedPtr<Xuzumi::PlatformService> mPlatform;
	Xuzumi::SharedPtr<Xuzumi::WindowFrame> mFrame;
};
