#pragma once

#include "Xuzumi/Input/KeyboardKey.hpp"
#include "Xuzumi/Platform.Win32/Win32SDK.hpp"

namespace Xuzumi::Internal
{
	class Win32KeyMapping
	{
	public:
		using Win32Key = DWORD;
		static constexpr Win32Key kInvalidWin32Key = 0x00;

		Win32KeyMapping();

		KeyboardKey Map(Win32Key nativeKey) const;
		Win32Key Map(KeyboardKey key) const;

	private:
		void SetupKeyMapping();
		void BindKeys(Win32Key nativeKey, KeyboardKey key);

		std::array<Win32Key, std::size_t(KeyboardKey::Count_)> mFromKeyboardKey = {};
		
		static constexpr std::size_t kMaxWin32Keys = 255;
		std::array<KeyboardKey, kMaxWin32Keys> mFromWin32Key = {};
	};
}
