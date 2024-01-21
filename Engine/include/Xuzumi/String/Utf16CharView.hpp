/**
 * @file Xuzumi/String/Utf16CharView.hpp
 *
 * @brief Defines the `Utf16CharView` class, designed for iterating over and
 * decoding UTF-16 sequences into Unicode code points.
 */
#pragma once

#include "Xuzumi/String/Utf16Facts.hpp"

namespace Xuzumi
{
	/**
	 * @brief Represents a view of a character encoded in UTF-16 format.
	 *
	 * This class iterates over a UTF-16 encoded sequence, decoding characters
	 * into Unicode code points. It maintains an immutable state; once created, a
	 * `Utf16CharView` object cannot be made to view another encoded sequence
	 * except through assignment.
	 */
	class Utf16CharView
	{
	public:
		/**
		 * @brief Constructs an empty character view.
		 *
		 * The character is considered invalid, and the `IsValid` method returns
		 * `false`.
		 */
		Utf16CharView() = default;

		/**
		 * @brief Constructs a view of the specified @p character.
		 *
		 * If @p character is `nullptr`, the view is empty.
		 *
		 * @param character A pointer to the character encoded in UTF-16 format.
		 */
		Utf16CharView(const char16_t* character);

		/**
		 * @brief Gets a pointer to the character viewed by `*this`.
		 *
		 * @return A pointer to the first code unit of the character viewed by
		 * `*this`.
		 */
		const char16_t* Get() const;

		/**
		 * @brief Constructs a view of the next character in a UTF-16 encoded
		 * sequence.
		 *
		 * @return A view of the next character provided that the current character
		 * is valid and is not terminating, otherwise, a copy of `*this`.
		 */
		Utf16CharView Next() const;

		/**
		 * @brief Constructs a view of the previous character in a UTF-16 encoded
		 * sequence.
		 *
		 * Internally, this method performs no bounds checks on whether the backward
		 * memory access in valid, so the client is responsible for the validation.
		 *
		 * @return A view to the previous character.
		 */
		Utf16CharView Previous() const;

		/**
		 * @brief Checks whether the current character is potentially valid.
		 *
		 * A character is potentially valid when the amount of code units can be
		 * monosemantically decoded.
		 *
		 * @retval true The character is potentially valid.
		 * @retval false The character is invalid and cannot be decoded.
		 */
		bool IsValid() const;

		/**
		 * @brief Checks whether the current character is a null-terminator.
		 *
		 * Internally, this method checks whether the viewed code unit is equal to
		 * zero.
		 *
		 * @retval true The character is a null-terminator.
		 * @retval false The character is not a null-terminator.
		 */
		bool IsTerminator() const;

		/**
		 * @brief Checks whether the @p other holds a view of the same memory as
		 * `*this`.
		 *
		 * @param other A UTF-16 character view.
		 *
		 * @retval true @p other holds a view of the same memory as `*this`.
		 * @retval false @p other and `*this` hold views of different memory
		 * locations.
		 */
		bool IsSameMemory(const Utf16CharView& other) const;

		/**
		 * @brief Decodes the UTF-16 encoded sequence into a Unicode code point.
		 *
		 * Internally, this method might iterate over code units that are further
		 * down in memory than the code unit `*this` points to, performing no
		 * boundary checks. Thus, the client is responsible for checking against
		 * under-encoded characters.
		 * If there was any error decoding the code units of the viewed character,
		 * the special Unicode code point is returned. The code point returned
		 * should be checked via the `UnicodeFacts::IsUnknownCharacter` method in
		 * order to determine whether a character is decoded successfully.
		 *
		 * @return The Unicode code point of the viewed character.
		 *
		 * @see Xuzumi::UnicodeFacts::IsUnknownCharacter
		 */
		char32_t DecodeCodePoint() const;

	private:
		std::size_t DecodeCodeUnitAmount() const;
		char16_t GetHighSurrogate() const;
		char16_t GetLowSurrogate() const;

		const char16_t* mCurrentCharacter = nullptr;
		std::size_t mCodeUnitAmount = Internal::Utf16Facts::kInvalidCodeUnitAmount;
	};
}
