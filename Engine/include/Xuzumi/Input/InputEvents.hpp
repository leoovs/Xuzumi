/**
 * @file Xuzumi/Input/InputEvents.hpp
 *
 * @brief Defines input events used in the engine.
 */

#pragma once

#include "Xuzumi/Messaging/Event.hpp"
#include "Xuzumi/Input/KeyboardKey.hpp"
#include "Xuzumi/String/UnicodeFacts.hpp"

namespace Xuzumi
{
	/**
	 * @brief Represents an event triggered when a keyboard key is pressed.
	 */
	class KeyDownEvent : public Event<KeyDownEvent>
	{
	public:
		/**
		 * @brief The key that was pressed.
		 */
		KeyboardKey KeyDown = KeyboardKey::Unknown;

		/**
		 * @brief Constructs a `KeyDownEvent` with the specified @p keyDown.
		 *
		 * @param keyDown The key that was pressed.
		 */
		KeyDownEvent(KeyboardKey keyDown)
			: KeyDown(keyDown)
		{}
	};

	/**
	 * @brief Represents an event triggered when a keyboard key is released.
	 */
	class KeyUpEvent : public Event<KeyUpEvent>
	{
	public:
		/**
		 * @brief The key that was released.
		 */
		KeyboardKey KeyUp = KeyboardKey::Unknown;

		/**
		 * @brief Constructs a `KeyUpEvent` with the specified @p keyUp.
		 *
		 * @param keyUp The key that was released.
		 */
		KeyUpEvent(KeyboardKey keyUp)
			: KeyUp(keyUp)
		{}
	};

	/**
	 * @brief Represents an event triggered on text input.
	 *
	 * Reports text input events in a keyboard-agnostic manner.
	 */
	class CharacterInputEvent : public Event<CharacterInputEvent>
	{
	public:
		/**
		 * @brief The resulting Unicode code point that encodes the input character.
		 */
		char32_t UnicodeCodePoint = UnicodeFacts::kWhiteSquareCharacterCodePoint;

		/**
		 * @brief Constructs a `CharacterInputEvent` with the specified
		 * @p unicodeCodePoint
		 *
		 * @param unicodeCodePoint The UTF-32 character.
		 */
		CharacterInputEvent(char32_t unicodeCodePoint)
			: UnicodeCodePoint(unicodeCodePoint)
		{}
	};
}
