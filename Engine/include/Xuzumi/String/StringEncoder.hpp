/**
 * @file Xuzumi/String/StringEncoder.hpp
 *
 * @brief Defines the `StringEncoder` class for encoding UTF text.
 */

#pragma once

#include "Xuzumi/Memory/ObserverPtr.hpp"
#include "Xuzumi/String/UtfTextReader.hpp"

namespace Xuzumi
{
	/**
	 * @brief Implements an encoder for encoding text in different UTF formats.
	 */
	class StringEncoder
	{
	public:
		/**
		 * @brief Represents the result of the encoding process.
		 */
		enum class Status
		{
			/// @brief The encoding process was not initiated.
			Idle,
			/// @brief The encoding process was successful.
			Success,
			/// @brief No UTF reader was provided; encoding process failed.
			ReaderNotSet,
			/// @brief Invalid character encountered; encoding process failed.
			InvalidCharacter
		};

		/**
		 * @brief Constructs a `StringEncoder` with `Idle` status.
		 */
		StringEncoder() = default;

		/**
		 * @brief Constructs a `StringEncoder` with the provided UTF reader.
		 *
		 * @param reader A UTF reader that streams characters to be encoded.
		 */
		StringEncoder(ObserverPtr<UtfTextReader> reader);

		/**
		 * @brief Set a custom UTF reader.
		 *
		 * @param reader A UTF reader that streams characters to be encoded.
		 */
		void SetReader(ObserverPtr<UtfTextReader> reader);

		/**
		 * @brief Encode text from the UTF reader in UTF-8 format.
		 *
		 * @param dest An insert iterator for the destination string where the
		 * encoded characters will be placed.
		 */
		void EncodeUtf8(std::insert_iterator<std::string> dest);

		/**
		 * @brief Encode text from the UTF reader in UTF-16 format.
		 *
		 * @param dest An insert iterator for the destination string where the
		 * encoded characters will be placed.
		 */
		void EncodeUtf16(std::insert_iterator<std::u16string> dest);

		/**
		 * @brief Query current status of an encoding process.
		 *
		 * @return A `Status` enum value representing the encoding process status.
		 */
		Status GetStatus() const;

	private:
		void ResetReader() const;

		ObserverPtr<UtfTextReader> mReader;
		Status mStatus = Status::Idle;
	};
}
