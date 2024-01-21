/**
 * @file Xuzumi/String/UnicodeFacts.hpp
 *
 * @brief Defines the `UnicodeFacts` which encapsulates a collection of
 * constants and methods related to Unicode.
 */
#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	/**
	 * @brief Encapsulates a collection of constants and procedures related to
	 * Unicode.
	 */
	struct UnicodeFacts
	{
		/**
		 * @brief The replacement character.
		 *
		 * Represents the `REPLACEMENT CHARACTER` at code point U+FFFD.
		 */
		static constexpr char32_t kReplacementCharacterCodePoint = 0xFFFD;

		/**
		 * @brief The white square character.
		 *
		 * Represents the `WHITE SQUARE` at code point U+25A1.
		 */
		static constexpr char32_t kWhiteSquareCharacterCodePoint = 0x25A1;

		/**
		 * @brief Checks whether the specified @p character may be classified as
		 * unknown.
		 *
		 * @retval true The character is either the replacement character or the
		 * white square character.
		 * @retval false The character is a readable Unicode character.
		 */
		static bool IsUnknownCharacter(char32_t character);
	};
}
