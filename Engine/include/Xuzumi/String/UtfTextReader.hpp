/**
 * @file Xuzumi/String/UtfTextReader.hpp
 *
 * @brief Defines the `UtfTextReader` hierarchy that provides common interface
 * for reading UTF encoded character sequences.
 */

#pragma once

#include "Xuzumi/String/UnicodeFacts.hpp"
#include "Xuzumi/String/Utf8CharView.hpp"
#include "Xuzumi/String/Utf16CharView.hpp"

namespace Xuzumi
{
	/**
	 * @brief This class represents basic UTF text reader interface.
	 */
	class UtfTextReader
	{
	public:
		/**
		 * @brief Release resources of `*this` if necessary.
		 */
		virtual ~UtfTextReader() = default;

		/**
		 * @brief Move to the next UTF encoded character.
		 */
		virtual void Advance() = 0;

		/**
		 * @brief Go back to the previous UTF encoded character.
		 */
		virtual void RollBack() = 0;

		/**
		 * @brief Set current character to be the beginning of a UTF encoded
		 * sequence.
		 */
		virtual void Reset() = 0;

		/**
		 * @brief Reads a UTF encoded character and decodes its Unicode code point.
		 *
		 * @return The Unicode code point of the UTF encoded character.
		 */
		virtual char32_t ReadCharacter() const = 0;

		/**
		 * @brief Checks whether the current character is valid.
		 *
		 * @retval true The current character is valid.
		 * @retval false The current character is invalid.
		 */
		virtual bool CharacterIsValid() const = 0;

		/**
		 * @brief Checks whether the current character is terminating.
		 *
		 * The terminating character marks the end of the UTF encoded text, the
		 * the character reading should be terminated.
		 *
		 * @retval true The current character is terminating.
		 * @retval false The current character is not terminating.
		 */
		virtual bool IsEnd() const = 0;
	};

	/**
	 * @brief Represents an adapter of the `Utf8CharView` for the `UtfTextReader`
	 * interface.
	 */
	class Utf8TextReader : public UtfTextReader
	{
	public:
		/**
		 * @brief Constructs a `Utf8TextReader`.
		 *
		 * @param begin The starting point of the text.
		 */
		Utf8TextReader(Utf8CharView begin);

		/**
		 * @brief Releases any acquired resources.
		 */
		~Utf8TextReader() override = default;

		/**
		 * @brief Move to the next UTF-8 character.
		 */
		void Advance() override;

		/**
		 * @brief Go back to the previous UTF-8 character.
		 */
		void RollBack() override;

		/**
		 * @brief Set current character to be the beginning of a UTF-8 encoded byte
		 * sequence.
		 */
		void Reset() override;

		/**
		 * @brief Reads a UTF-8 character and decodes its Unicode code point.
		 *
		 * @return The Unicode code point of the UTF-8 encoded character.
		 */
		char32_t ReadCharacter() const override;

		/**
		 * @brief Checks whether the current UTF-8 character is valid.
		 *
		 * @retval true The current UTF-8 character is valid.
		 * @retval false The current UTF-8 character is invalid.
		 */
		bool CharacterIsValid() const override;

		/**
		 * @brief Checks whether the current UTF-8 character is terminating.
		 *
		 * @retval true The current character is terminating.
		 * @retval false The current character is not terminating.
		 */
		bool IsEnd() const override;

	private:
	 	Utf8CharView mBegin;
		Utf8CharView mCurrentCharacter;
	};

	/**
	 * @brief Represents an adapter of the `Utf16CharView` for the
	 * `UtfTextReader` interface.
	 */
	class Utf16TextReader : public UtfTextReader
	{
	public:
		/**
		 * @brief Constructs a `Utf16TextReader`.
		 *
		 * @param begin The starting point of the text.
		 */
		Utf16TextReader(Utf16CharView begin);

		/**
		 * @brief Releases any acquired resources.
		 */
		~Utf16TextReader() override = default;

		/**
		 * @brief Move to the next UTF-16 character.
		 */
		void Advance() override;

		/**
		 * @brief Go back to the previous UTF-16 character.
		 */
		void RollBack() override;

		/**
		 * @brief Set current character to the beginning of a UTF-16 encoded
		 * string.
		 */
		void Reset() override;

		/**
		 * @brief Reads a UTF-16 character and decodes its Unicode code point.
		 *
		 * @return The Unicode code point of the UTF-16 encoded character.
		 */
		char32_t ReadCharacter() const override;

		/**
		 * @brief Checks whether the current UTF-16 character is valid.
		 *
		 * @retval true The current UTF-16 character is valid.
		 * @retval false The current UTF-16 character is invalid.
		 */
		bool CharacterIsValid() const override;

		/**
		 * @brief Checks whether the current UTF-16 character is terminating.
		 *
		 * @retval true The current character is terminating.
		 * @retval false The current character is not terminating.
		 */
		bool IsEnd() const override;

	private:
		Utf16CharView mBegin;
		Utf16CharView mCurrentCharacter;
	};

	/**
	 * @brief Represents a UTF-32 string adapter for the `UtfTextReader`
	 * interface.
	 *
	 * Does not perform any decoding, simply views the UTF-32 .
	 */
	class Utf32TextReader : public UtfTextReader
	{
	public:
		/**
		 * @brief Constructs a `Utf32TextReader`.
		 *
		 * @param begin The starting point of the text.
		 */
		Utf32TextReader(const char32_t* begin);

		/**
		 * @brief Releases any acquired resources.
		 */
		~Utf32TextReader() override = default;

		/**
		 * @brief Moves to the next UTF-32 code unit.
		 */
		void Advance() override;

		/**
		 * @brief Movies to the previous UTF-32 code unit.
		 */
		void RollBack() override;

		/**
		 * @brief Set current character to the beginning of a UTF-32 encoded
		 * string.
		 */
		void Reset() override;

		/**
		 * @brief Dereferences current character view and obtains UTF-32 code unit.
		 *
		 * @return A UTF-32 code unit.
		 */
		char32_t ReadCharacter() const override;

		/**
		 * @brief Check whether the current character is valid.
		 *
		 * @retval true The current character is valid.
		 * @retval false The current character is invalid.
		 */
		bool CharacterIsValid() const override;

		/**
		 * @brief Check whether the current character is terminating.
		 *
		 * @retval true The current character is terminating.
		 * @retval false The current character is not terminating.
		 */
		bool IsEnd() const override;

	private:
		const char32_t* mBegin = nullptr;
		const char32_t* mCurrentCharacter = nullptr;
	};
}
