#include "Xuzumi/Core/PlatformInfo.hpp"

#if XZ_PLATFORM_IS(WIN32)

#include "Xuzumi/Input.Win32/Win32KeyMapping.hpp"

namespace Xuzumi::Internal
{
	Win32KeyMapping::Win32KeyMapping()
	{
		mFromWin32Key.fill(KeyboardKey::Unknown);
		mFromKeyboardKey.fill(kInvalidWin32Key);

		SetupKeyMapping();
	}

	KeyboardKey Win32KeyMapping::Map(Win32Key nativeKey) const
	{
		std::size_t index(nativeKey);
		return mFromWin32Key[index];
	}

	Win32KeyMapping::Win32Key Win32KeyMapping::Map(KeyboardKey key) const
	{
		auto index = static_cast<std::size_t>(key);
		return mFromKeyboardKey[index];
	}

	void Win32KeyMapping::SetupKeyMapping()
	{
		BindKeys(kInvalidWin32Key, KeyboardKey::Unknown);

		BindKeys(Win32Key(0x30), KeyboardKey::Numeric0);
		BindKeys(Win32Key(0x31), KeyboardKey::Numeric1);
		BindKeys(Win32Key(0x32), KeyboardKey::Numeric2);
		BindKeys(Win32Key(0x33), KeyboardKey::Numeric3);
		BindKeys(Win32Key(0x34), KeyboardKey::Numeric4);
		BindKeys(Win32Key(0x35), KeyboardKey::Numeric5);
		BindKeys(Win32Key(0x36), KeyboardKey::Numeric6);
		BindKeys(Win32Key(0x37), KeyboardKey::Numeric7);
		BindKeys(Win32Key(0x38), KeyboardKey::Numeric8);
		BindKeys(Win32Key(0x39), KeyboardKey::Numeric9);

		BindKeys(Win32Key(0x41), KeyboardKey::A);
		BindKeys(Win32Key(0x42), KeyboardKey::B);
		BindKeys(Win32Key(0x43), KeyboardKey::C);
		BindKeys(Win32Key(0x44), KeyboardKey::D);
		BindKeys(Win32Key(0x45), KeyboardKey::E);
		BindKeys(Win32Key(0x46), KeyboardKey::F);
		BindKeys(Win32Key(0x47), KeyboardKey::G);
		BindKeys(Win32Key(0x48), KeyboardKey::H);
		BindKeys(Win32Key(0x49), KeyboardKey::I);
		BindKeys(Win32Key(0x4A), KeyboardKey::J);
		BindKeys(Win32Key(0x4B), KeyboardKey::K);
		BindKeys(Win32Key(0x4C), KeyboardKey::L);
		BindKeys(Win32Key(0x4D), KeyboardKey::M);
		BindKeys(Win32Key(0x4E), KeyboardKey::N);
		BindKeys(Win32Key(0x4F), KeyboardKey::O);
		BindKeys(Win32Key(0x50), KeyboardKey::P);
		BindKeys(Win32Key(0x51), KeyboardKey::Q);
		BindKeys(Win32Key(0x52), KeyboardKey::R);
		BindKeys(Win32Key(0x53), KeyboardKey::S);
		BindKeys(Win32Key(0x54), KeyboardKey::T);
		BindKeys(Win32Key(0x55), KeyboardKey::U);
		BindKeys(Win32Key(0x56), KeyboardKey::V);
		BindKeys(Win32Key(0x57), KeyboardKey::W);
		BindKeys(Win32Key(0x58), KeyboardKey::X);
		BindKeys(Win32Key(0x59), KeyboardKey::Y);
		BindKeys(Win32Key(0x5A), KeyboardKey::Z);
	
		BindKeys(VK_ESCAPE, KeyboardKey::Escape);
		BindKeys(VK_OEM_3, KeyboardKey::Grave);
		BindKeys(VK_TAB, KeyboardKey::Tab);
		BindKeys(VK_CAPITAL, KeyboardKey::CapsLock);
		BindKeys(VK_LSHIFT, KeyboardKey::LeftShift);
		BindKeys(VK_LCONTROL, KeyboardKey::LeftCtrl);
		BindKeys(VK_LMENU, KeyboardKey::LeftAlt);
		BindKeys(VK_RSHIFT, KeyboardKey::RightShift);
		BindKeys(VK_RCONTROL, KeyboardKey::RightCtrl);
		BindKeys(VK_RMENU, KeyboardKey::RightAlt);
		BindKeys(VK_BACK, KeyboardKey::Backspace);
		BindKeys(VK_RETURN, KeyboardKey::Enter);
		BindKeys(VK_UP, KeyboardKey::UpArrow);
		BindKeys(VK_DOWN, KeyboardKey::DownArrow);
		BindKeys(VK_LEFT, KeyboardKey::LeftArrow);
		BindKeys(VK_RIGHT, KeyboardKey::RightArrow);
	}

	void Win32KeyMapping::BindKeys(
		Win32Key nativeKey,
		KeyboardKey key
	)
	{
		mFromWin32Key[std::size_t(nativeKey)] = key; 	
		mFromKeyboardKey[std::size_t(key)] = nativeKey;
	}
}

#endif // XZ_PLATFORM_IS(WIN32)
