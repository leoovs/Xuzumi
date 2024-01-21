/**
 * @file Xuzumi/String/Utf8CharView.hpp
 *
 * @brief Defines the `Utf8CharView` class, designed for iterating over and
 * decoding UTF-8 byte sequences into Unicode code points.
 */
#pragma once

#include "Xuzumi/String/Utf8Facts.hpp"

namespace Xuzumi
{
	/**
	 * @brief Represents a view of a character encoded in UTF-8 format.
	 *
	 * This class iterates over a UTF-8 byte sequence, decoding characters into
	 * Unicode code points. It maintains an immutable state; once created, a
	 * `Utf8CharView` object cannot be made to view another byte sequence except
	 * through assignment.
	 */
	class Utf8CharView
	{
	public:
		/**
		 * @brief Constructs an empty character view.
		 *
		 * The character is considered invalid, and the `IsValid` method returns
		 * `false`.
		 */
		Utf8CharView() = default;

		/**
		 * @brief Constructs a view of the specified @p character.
		 *
		 * If @p character is `nullptr`, the view is empty.
		 *
		 * @param character A pointer to the first code unit of a character encoded
		 * in UTF-8 format.
		 */
		Utf8CharView(const char* character);

		/**
		 * @brief Constructs a view on the specified @p character.
		 *
		 * If @p character is `nullptr`, the view is empty.
		 *
		 * @param character A pointer to the first code unit of a character encoded
		 * in UTF-8 format.
		 */
		Utf8CharView(const std::uint8_t* character);

		/**
		 * @brief Gets a pointer to the character viewed by `*this`.
		 *
		 * @return A pointer to the first code unit of the character viewed by
		 * `*this`.
		 */
		const std::uint8_t* Get() const;

		/**
		 * @brief Constructs a view of the next character in a UTF-8 byte sequence.
		 *
		 * @return A view of the next character provided that the current character
		 * is valid and is not terminating, otherwise, a copy of `*this`.
		 */
		Utf8CharView Next() const;

		/**
		 * @brief Constructs a view of the previous character in a UTF-8 byte
		 * sequence.
		 *
		 * Internally, this method performs no bounds checks on whether the backward
		 * memory access in valid, so the client is responsible for the validation.
		 *
		 * @return A view to the previous character.
		 */
		Utf8CharView Previous() const;

		/**
		 * @brief Checks whether the current character is potentially valid.
		 *
		 * A character is potentially valid when its leader byte encodes the
		 * acceptable amount of code units, meaning that it has the potential to be
		 * decoded properly.
		 *
		 * @retval true The character is potentially valid.
		 * @retval false The character is invalid and cannot be decoded.
		 */
		bool IsValid() const;

		/**
		 * @brief Checks whether the current character is a null-terminator.
		 *
		 * Internally, this method checks whether the viewed byte stored zero.
		 *
		 * @retval true The character is a null-terminator.
		 * @retval false The character is not a null-terminator.
		 */
		bool IsTerminator() const;

		/**
		 * @brief Checks whether the @p other holds a view of the same memory as
		 * `*this`.
		 *
		 * @param other A UTF-8 character view.
		 *
		 * @retval true @p other holds a view of the same memory as `*this`.
		 * @retval false @p other and `*this` hold views of different memory
		 * locations.
		 */
		bool IsSameMemory(const Utf8CharView& other) const;

		/**
		 * @brief Decodes the UTF-8 byte sequence into a Unicode code point.
		 *
		 * Internally, this method might iterate over bytes that are further down in
		 * memory than the byte `*this` points to, performing no boundary checks.
		 * Thus, the client is responsible for checking against under-encoded
		 * characters.
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

		const std::uint8_t* mCurrentCharacter = nullptr;
		std::size_t mCodeUnitAmount = Internal::Utf8Facts::kInvalidCodeUnitAmount;
	};
}
