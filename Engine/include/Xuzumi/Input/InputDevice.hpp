/**
 * @file Xuzumi/Input/InputDevice.hpp
 *
 * @brief Defines the abstract class `InputDevice`.
 */

#pragma once

#include "Xuzumi/Input/KeyboardKey.hpp"

namespace Xuzumi
{
	/**
	 * @brief Represents a logical input device.
	 *
	 * This class serves as a logical input device that captures and manages
	 * input-related data.
	 */
	class InputDevice
	{
	public:
		/**
		 * @brief Performs platform-specific clean-up of input device resources.
		 */
		virtual ~InputDevice() = default;

		/**
		 * @brief Checks whether the specified @p key is currently being pressed.
		 *
		 * @param key A virtual key.
		 *
		 * @retval true The key is currently pressed.
		 * @retval false The key is currently released.
		 */
		virtual bool GetKeyState(KeyboardKey key) const = 0;
	};
}
