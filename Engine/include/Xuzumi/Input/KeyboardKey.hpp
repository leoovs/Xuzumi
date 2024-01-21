/**
 * @file Xuzumi/Input/KeyboardKey.hpp
 *
 * @brief Defines the `KeyboardKey` enumeration, representing the keyboard keys
 * in the engine.
 */

#pragma once

namespace Xuzumi
{
	/**
	 * @brief Enumeration of common keyboard keys.
	 *
	 * This enumeration defines a set of keys that are commonly found on modern
	 * keyboards.
	 */
	enum class KeyboardKey
	{
		Unknown,

		Numeric0,
		Numeric1,
		Numeric2,
		Numeric3,
		Numeric4,
		Numeric5,
		Numeric6,
		Numeric7,
		Numeric8,
		Numeric9,

		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W,
		X, Y, Z,

		Escape,
		Grave,
		Tab,
		CapsLock,
		LeftShift,
		LeftCtrl,
		LeftAlt,
		RightShift,
		RightCtrl,
		RightAlt,
		Backspace,
		Enter,
		UpArrow,
		DownArrow,
		LeftArrow,
		RightArrow,

		Count_
	};
}
