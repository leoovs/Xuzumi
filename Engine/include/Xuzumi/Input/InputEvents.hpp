#pragma once

#include "Xuzumi/Messaging/Event.hpp"
#include "Xuzumi/Input/KeyboardKey.hpp"
#include "Xuzumi/String/UnicodeFacts.hpp"

namespace Xuzumi
{
	class KeyDownEvent : public Event<KeyDownEvent>
	{
	public:
		KeyboardKey KeyDown = KeyboardKey::Unknown;

		KeyDownEvent() = default;

		KeyDownEvent(KeyboardKey keyDown)
			: KeyDown(keyDown)
		{}
	};

	class KeyUpEvent : public Event<KeyUpEvent>
	{
	public:
		KeyboardKey KeyUp = KeyboardKey::Unknown;

		KeyUpEvent() = default;

		KeyUpEvent(KeyboardKey keyUp)
			: KeyUp(keyUp)
		{}
	};

	class CharacterInputEvent : public Event<CharacterInputEvent>
	{
	public:
		char32_t UnicodeCodePoint = UnicodeFacts::kWhiteSquareCharacterCodePoint;
	
		CharacterInputEvent() = default;

		CharacterInputEvent(char32_t unicodeCodePoint)
			: UnicodeCodePoint(unicodeCodePoint)
		{}
	};
}
