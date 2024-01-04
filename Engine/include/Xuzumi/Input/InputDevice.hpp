#pragma once

#include "Xuzumi/Input/KeyboardKey.hpp"

namespace Xuzumi
{
	class InputDevice
	{
	public:
		virtual ~InputDevice() = default;

		virtual bool GetKeyState(KeyboardKey key) const = 0;
	};
}
