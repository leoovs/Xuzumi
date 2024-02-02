#include "App.hpp"

App::App()
{
	XZ_PROFILE_FUNCTION();

	auto events = Xuzumi::ObserverPtr<Xuzumi::EventBus>(&mEvents);

	mEventSubscriber.Subscribe(events);

	mEventSubscriber
		.BeginThis(this)
			.Method(&App::OnFrameClose)
			.Method(&App::OnFrameResize)
			.Method(&App::OnKeyDown)
			.Method(&App::OnKeyUp)
			.Method(&App::OnChar)
		.EndThis();

	mPlatform = Xuzumi::CreatePlatformService();
	mPlatform->Connect(events);

	Xuzumi::WindowFrameSpecification frameSpec;
	frameSpec.Caption = u8"Caption | Заголовок";
	frameSpec.Resizable = true;
	frameSpec.Visible = true;

	mFrame = mPlatform->CreateWindowFrame(frameSpec);

	XZ_LOG(XZ_APP_LOGGER, Info, "%s", mPlatform.GetResourceTypeInfo().Name.data());
	XZ_LOG(XZ_APP_LOGGER, Info, "%s", mFrame.GetResourceTypeInfo().Name.data());
	XZ_LOG(XZ_APP_LOGGER, Info, "%s", mPlatform->GetInputDevice().GetResourceTypeInfo().Name.data());
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
	XZ_PROFILE_FUNCTION();

	{ XZ_PROFILE_BLOCK("App: Poll platform events");
		mPlatform->PollEvents();
	}
	{ XZ_PROFILE_BLOCK("App: Dispatch all events");
		mEvents.Dispatch();
	}
}

void App::Quit()
{
	XZ_PROFILE_FUNCTION();

	mRunning = false;
}

bool App::OnFrameClose(const Xuzumi::WindowFrameClosedEvent& event)
{
	XZ_PROFILE_FUNCTION();

	if (mFrame.Get() == event.ClosedFrame.Get())
	{
		Quit();
	}

	return true;
}

bool App::OnFrameResize(const Xuzumi::WindowFrameResizedEvent& event)
{
	XZ_PROFILE_FUNCTION();

	XZ_LOG(XZ_APP_LOGGER, Info, "New size: %u, %u", event.Width, event.Height);
	return true;
}

bool App::OnKeyDown(const Xuzumi::KeyDownEvent& event)
{
	XZ_PROFILE_FUNCTION();

	if (event.KeyDown == Xuzumi::KeyboardKey::Escape)
	{
		mFrame->SetCaption("");
	}

	auto inputDevice = mPlatform->GetInputDevice();

	XZ_LOG(XZ_APP_LOGGER, Info, "Key down: %d", event.KeyDown);

	return true;
}

bool App::OnKeyUp(const Xuzumi::KeyUpEvent& event)
{
	XZ_PROFILE_FUNCTION();

	XZ_LOG(XZ_APP_LOGGER, Info, "Key up: %d", event.KeyUp);

	return true;
}

bool App::OnChar(const Xuzumi::CharacterInputEvent& event)
{
	XZ_PROFILE_FUNCTION();

	XZ_LOG(XZ_APP_LOGGER, Info, "Char: %d (0x%x)", event.UnicodeCodePoint, event.UnicodeCodePoint);

	char32_t text[]{ event.UnicodeCodePoint, U'\0' };
	Xuzumi::Utf32TextReader reader(text);

	std::string ch;
	Xuzumi::StringEncoder(Xuzumi::ObserverPtr<Xuzumi::UtfTextReader>(&reader))
		.EncodeUtf8(std::inserter(ch, ch.begin()));

	mFrame->SetCaption(mFrame->GetSpecification().Caption + ch);

	return true;
}

