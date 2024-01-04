#pragma once

#include "Xuzumi/Messaging/Event.hpp"
#include "Xuzumi/Input/KeyboardKey.hpp"

namespace Xuzumi
{
	class KeyDownEvent : public Event<KeyDownEvent>
	{
	public:
		KeyboardKey KeyDown;

		KeyDownEvent(KeyboardKey keyDown)
			: KeyDown(keyDown)
		{}
	};

	class KeyUpEvent : public Event<KeyUpEvent>
	{
	public:
		KeyboardKey KeyUp;

		KeyUpEvent(KeyboardKey keyUp)
			: KeyUp(keyUp)
		{}
	};
}
