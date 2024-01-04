#include "Xuzumi/Core/PlatformInfo.hpp"

#if XZ_PLATFORM_IS(WIN32)

#include "Xuzumi/Input.Win32/Win32InputDevice.hpp"

#include "Xuzumi/Input/InputEvents.hpp"

namespace Xuzumi::Internal
{
	Win32InputDevice::Win32InputDevice()
	{
		mKeyboardState.fill(false);
	}

	bool Win32InputDevice::GetKeyState(KeyboardKey key) const
	{
		return mKeyboardState[std::size_t(key)];
	}

	const Win32KeyMapping& Win32InputDevice::GetKeyMapping() const
	{
		return mKeyMapping;
	}

	void Win32InputDevice::Connect(ObserverPtr<EventBus> keyboardEvents)
	{
		mInputEventsPublisher.Connect(keyboardEvents);

		if (!keyboardEvents)
		{
			return;
		}

		mPlatformEventsSubscriber.Subscribe(keyboardEvents)
			.BeginThis(this)
				.Method(&Win32InputDevice::OnWin32KeyDown)
				.Method(&Win32InputDevice::OnWin32KeyUp)
			.EndThis();
	}

	WPARAM Win32InputDevice::MapLeftRightKeys(WPARAM wParam, LPARAM lParam)
	{
		UINT scancode = (lParam & 0x00FF0000) >> 16;
		bool isExtendedKey = (lParam & 0x01000000) != 0;
	
		switch (wParam)
		{
		case VK_SHIFT:
			return MapVirtualKeyA(scancode, MAPVK_VSC_TO_VK_EX);
		case VK_CONTROL:
			return isExtendedKey ? VK_RCONTROL : VK_LCONTROL;
		case VK_MENU:
			return isExtendedKey ? VK_RMENU : VK_LMENU;
		}
	
		return wParam;
	}

	bool Win32InputDevice::OnWin32KeyDown(const Win32KeyDownEvent& event)
	{
		LPARAM lParam = event.MessageLParam;
		WPARAM wParam = MapLeftRightKeys(event.MessageWParam, lParam);

		Win32KeyMapping::Win32Key nativeKey(wParam);
		KeyboardKey keyDown = mKeyMapping.Map(nativeKey);
	
		mKeyboardState[std::size_t(keyDown)] = true;
		mInputEventsPublisher.Publish<KeyDownEvent>(keyDown);
	
		return true;
	}

	bool Win32InputDevice::OnWin32KeyUp(const Win32KeyUpEvent& event)
	{
		LPARAM lParam = event.MessageLParam;
		WPARAM wParam = MapLeftRightKeys(event.MessageWParam, lParam);
	
		Win32KeyMapping::Win32Key nativeKey(wParam);
		KeyboardKey keyUp = mKeyMapping.Map(nativeKey);
	
		mKeyboardState[std::size_t(keyUp)] = false;
		mInputEventsPublisher.Publish<KeyUpEvent>(keyUp);
	
		return true;
	}
}

#endif // XZ_PLATFORM_IS(WIN32)
