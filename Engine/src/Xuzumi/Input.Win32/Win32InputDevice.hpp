#pragma once

#include "Xuzumi/Platform.Win32/Win32SDK.hpp"
#include "Xuzumi/Messaging/EventSubscriber.hpp"
#include "Xuzumi/Messaging/EventPublisher.hpp"
#include "Xuzumi/Input/InputDevice.hpp"
#include "Xuzumi/Input.Win32/Win32KeyMapping.hpp"
#include "Xuzumi/Input.Win32/Win32InputEvents.hpp"

namespace Xuzumi::Internal
{
	class Win32InputDevice : public InputDevice
	{
	public:
		Win32InputDevice();
		~Win32InputDevice() override = default;

		bool GetKeyState(KeyboardKey key) const override;

		const Win32KeyMapping& GetKeyMapping() const;

		void Connect(ObserverPtr<EventBus> platformEvents);

	private:
		WPARAM MapLeftRightKeys(WPARAM wParam, LPARAM lParam);
		void SetKeyState(KeyboardKey key, bool state);

		bool OnWin32KeyDown(const Win32KeyDownEvent& event);
		bool OnWin32KeyUp(const Win32KeyUpEvent& event);

		std::array<bool, std::size_t(KeyboardKey::Count_)> mKeyboardState;
		Win32KeyMapping mKeyMapping;

		EventSubscriber mPlatformEventsSubscriber;
		EventPublisher mInputEventsPublisher;
	};
}
